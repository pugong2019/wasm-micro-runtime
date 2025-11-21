#ifndef SIMD_TEST_HELPER_H
#define SIMD_TEST_HELPER_H

#include "gtest/gtest.h"
#include "../../../../core/iwasm/include/wasm_export.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <unistd.h>
#include <cerrno>

// Forward declaration of getWASMFilename function
inline const char* getWASMFilename(const char* wasm_file);

// Helper class for RAII pattern with WAMR runtime (specific to SIMD tests)
class SIMDWAMRRuntimeRAII {
public:
    SIMDWAMRRuntimeRAII(uint32_t heap_size = 512 * 1024)
        : module_(nullptr), module_inst_(nullptr), exec_env_(nullptr) {
        // This class will NOT initialize the runtime globally
        // We assume the runtime is initialized by the test fixture
    }
    
    ~SIMDWAMRRuntimeRAII() {
        // Cleanup resources in reverse order
        if (exec_env_) {
            wasm_runtime_destroy_exec_env(exec_env_);
            exec_env_ = nullptr;
        }
        
        if (module_inst_) {
            wasm_runtime_deinstantiate(module_inst_);
            module_inst_ = nullptr;
        }
        
        if (module_) {
            wasm_runtime_unload(module_);
            module_ = nullptr;
        }
        // Do NOT destroy the runtime here, it's managed by the test fixture
    }
    
    // Load and instantiate a WASM module
    bool load_module(const char *wasm_file_path, uint32_t stack_size = 64 * 1024, uint32_t heap_size = 512 * 1024) {
        // 添加调试信息
        printf("Attempting to load WASM module from path: %s\n", wasm_file_path);
        
        // 检查文件是否存在
        if (access(wasm_file_path, F_OK) != 0) {
            printf("WASM file does not exist: %s\n", wasm_file_path);
        }
        
        // Read WASM file
        FILE *file = fopen(wasm_file_path, "rb");
        if (!file) {
            printf("Failed to open WASM file: %s (error: %s)\n", wasm_file_path, strerror(errno));
            return false;
        }
        
        // Get file size
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        // Allocate buffer and read file
        uint8_t *wasm_buf = (uint8_t*)malloc(file_size);
        if (!wasm_buf) {
            printf("Failed to allocate memory for WASM file\n");
            fclose(file);
            return false;
        }
        
        size_t bytes_read = fread(wasm_buf, 1, file_size, file);
        fclose(file);
        
        if (bytes_read != (size_t)file_size) {
            printf("Failed to read WASM file completely\n");
            free(wasm_buf);
            return false;
        }
        
        // Load module
        char error_buf[128];
        module_ = wasm_runtime_load(wasm_buf, file_size, error_buf, sizeof(error_buf));
        if (!module_) {
            printf("Failed to load WASM module: %s\n", error_buf);
            free(wasm_buf);
            return false;
        }
        
        // Instantiate module
        module_inst_ = wasm_runtime_instantiate(module_, stack_size, heap_size, error_buf, sizeof(error_buf));
        if (!module_inst_) {
            printf("Failed to instantiate WASM module: %s\n", error_buf);
            free(wasm_buf);
            return false;
        }
        
        // Create execution environment
        exec_env_ = wasm_runtime_create_exec_env(module_inst_, stack_size);
        if (!exec_env_) {
            printf("Failed to create execution environment\n");
            free(wasm_buf);
            return false;
        }
        
        // Free WASM buffer as it's no longer needed
        free(wasm_buf);
        return true;
    }
    

    

    
    // Call a WASM function
    bool call_function(const char *function_name, uint32_t argc, uint32_t *argv) {
        printf("Module instance: %p, Exec env: %p\n", module_inst_, exec_env_);
        if (!module_inst_ || !exec_env_) {
            printf("Module not loaded or instantiated\n");
            return false;
        }
        
        // Lookup function
        printf("Looking up function: %s\n", function_name);
        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst_, function_name);
        if (!func) {
            printf("Failed to find function: %s\n", function_name);
            return false;
        }
        
        // Call function
        bool success = wasm_runtime_call_wasm(exec_env_, func, argc, argv);
        if (!success) {
            const char *exception = wasm_runtime_get_exception(module_inst_);
            printf("Exception occurred during WASM function call: %s\n", exception ? exception : "Unknown error");
        }
        
        return success;
    }
    
    // Get module instance
    wasm_module_inst_t get_module_inst() {
        return module_inst_;
    }
    
private:
    wasm_module_t module_;
    wasm_module_inst_t module_inst_;
    wasm_exec_env_t exec_env_;
};

class SIMDTestBase : public testing::Test {
protected:
    void SetUp() override {
        // Initialize runtime once per test fixture
        RuntimeInitArgs init_args;
        memset(&init_args, 0, sizeof(RuntimeInitArgs));
        init_args.mem_alloc_type = Alloc_With_System_Allocator;
        init_args.running_mode = Mode_Interp;
        ASSERT_TRUE(wasm_runtime_full_init(&init_args)) << "Failed to initialize WAMR runtime";
    }

    void TearDown() override {
        // Destroy runtime after each test
        wasm_runtime_destroy();
    }

    bool execute_wasm_function(const char* wasm_file, const char* func_name, 
                             void* params, uint32_t param_count, 
                             void* results, uint32_t result_count) {
        // 使用RAII模式创建运行时实例管理对象
        SIMDWAMRRuntimeRAII runtime;
        
        // 获取完整的WASM文件路径
        const char* full_path = getWASMFilename(wasm_file);
        
        // 加载模块
        if (!runtime.load_module(full_path)) {
            return false;
        }
        
        // 获取模块实例
        wasm_module_inst_t module_inst = runtime.get_module_inst();
        if (!module_inst) {
            return false;
        }
        
        // 准备参数 - 为SIMD测试特别处理参数和结果
        uint32_t argv[2];
        void* params_native_addr = nullptr;
        void* results_native_addr = nullptr;
        
        // 为参数在WASM内存中分配空间
        if (param_count > 0 && params) {
            if (!wasm_runtime_module_malloc(module_inst, param_count * sizeof(uint32_t), &params_native_addr)) {
                printf("Failed to allocate memory for params in WASM module\n");
                return false;
            }
            // 复制参数到WASM内存
            memcpy(params_native_addr, params, param_count * sizeof(uint32_t));
            argv[0] = (uint32_t)((char*)params_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
        } else {
            argv[0] = 0;
        }
        
        // 为结果在WASM内存中分配空间
        if (result_count > 0) {
            if (!wasm_runtime_module_malloc(module_inst, result_count * sizeof(uint32_t), &results_native_addr)) {
                printf("Failed to allocate memory for results in WASM module\n");
                if (params_native_addr) {
                    wasm_runtime_module_free(module_inst, argv[0]);
                }
                return false;
            }
            argv[1] = (uint32_t)((char*)results_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
        } else {
            argv[1] = 0;
        }
        
        // 调用函数 - 假设函数签名是 (params_ptr, results_ptr)
  // 调用函数
    printf("Attempting to call function: %s with %d arguments\n", func_name, 2);
    bool success = runtime.call_function(func_name, 2, argv);
        
        // 复制结果回本地内存
        if (success && result_count > 0 && results && results_native_addr) {
            memcpy(results, results_native_addr, result_count * sizeof(uint32_t));
        }
        
        // 释放WASM内存
        if (param_count > 0 && params_native_addr) {
            wasm_runtime_module_free(module_inst, argv[0]);
        }
        if (result_count > 0 && results_native_addr) {
            wasm_runtime_module_free(module_inst, argv[1]);
        }
        
        return success;
    }

    char error_buf[128];
    
    // 8参数版本的execute_wasm_function函数重载
    bool execute_wasm_function(const char* wasm_file, const char* func_name,
                             void* params, uint32_t param_count,
                             void* results, uint32_t result_count,
                             void* extra_data, uint32_t extra_size) {
        // 创建运行时实例管理对象
        SIMDWAMRRuntimeRAII runtime;
        
        // 获取完整的WASM文件路径
        const char* full_path = getWASMFilename(wasm_file);
        
        // 加载模块
        if (!runtime.load_module(full_path)) {
            return false;
        }
        
        // 获取模块实例
        wasm_module_inst_t module_inst = runtime.get_module_inst();
        if (!module_inst) {
            return false;
        }
        
        // 准备参数 - 处理params、results和额外参数
        uint32_t argv[4];
        void* params_native_addr = nullptr;
        void* results_native_addr = nullptr;
        
        // 为参数在WASM内存中分配空间
        if (param_count > 0 && params) {
            if (!wasm_runtime_module_malloc(module_inst, param_count * sizeof(uint32_t), &params_native_addr)) {
                printf("Failed to allocate memory for params in WASM module\n");
                return false;
            }
            // 复制参数到WASM内存
            memcpy(params_native_addr, params, param_count * sizeof(uint32_t));
            argv[0] = (uint32_t)((char*)params_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
        } else {
            argv[0] = 0;
        }
        
        // 为结果在WASM内存中分配空间
        if (result_count > 0) {
            if (!wasm_runtime_module_malloc(module_inst, result_count * sizeof(uint32_t), &results_native_addr)) {
                printf("Failed to allocate memory for results in WASM module\n");
                if (params_native_addr) {
                    wasm_runtime_module_free(module_inst, argv[0]);
                }
                return false;
            }
            argv[1] = (uint32_t)((char*)results_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
        } else {
            argv[1] = 0;
        }
        
        // 设置额外参数
        if (extra_size >= sizeof(uint32_t) * 2 && extra_data) {
            uint32_t* extra_params = static_cast<uint32_t*>(extra_data);
            argv[2] = extra_params[0];
            argv[3] = extra_params[1];
        } else {
            argv[2] = 0;
            argv[3] = 0;
        }
        
        // 调用函数 - 假设函数签名是 (params_ptr, results_ptr, extra1, extra2)
        bool success = runtime.call_function(func_name, 4, argv);
        
        // 复制结果回本地内存
        if (success && result_count > 0 && results && results_native_addr) {
            memcpy(results, results_native_addr, result_count * sizeof(uint32_t));
        }
        
        // 释放WASM内存
        if (param_count > 0 && params_native_addr) {
            wasm_runtime_module_free(module_inst, argv[0]);
        }
        if (result_count > 0 && results_native_addr) {
            wasm_runtime_module_free(module_inst, argv[1]);
        }
        
        return success;
    }
    
    // 移除类内的getWASMFilename函数，避免与全局函数冲突
    
    // 接受std::string参数的execute_wasm_function函数重载
    bool execute_wasm_function(const std::string& wasm_file, const char* func_name,
                             void* param_types, void* params, uint32_t param_count,
                             void* ret_types, void* ret, uint32_t ret_count) {
        // 委托给const char*版本
        return execute_wasm_function(wasm_file.c_str(), func_name, params, param_count, ret, ret_count);
    }
};

// 函数前向声明
const char* getWASMFilename(const char *filename);

// 全局函数重载，接受8个参数 - 直接实现WAMR运行时的初始化、模块加载和函数调用

inline bool execute_wasm_function(const char *wasm_file, const char *function_name,
                          void *params, void *results, int32_t param_size, int32_t result_size) {
    // 初始化WAMR运行时
    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
    init_args.mem_alloc_type = Alloc_With_System_Allocator;
    
    if (!wasm_runtime_full_init(&init_args)) {
        printf("Failed to initialize WAMR runtime\n");
        return false;
    }
    
    bool result = false;
    
    // 尝试加载WASM模块
    uint32_t stack_size = 64 * 1024;  // 64KB栈大小
    uint32_t heap_size = 512 * 1024;  // 512KB堆大小
    wasm_module_t module = NULL;
    wasm_module_inst_t module_inst = NULL;
    wasm_exec_env_t exec_env = NULL;
    
    // 获取WASM文件路径
    const char *file_path = getWASMFilename(wasm_file);
    
    // 加载WASM模块
    FILE *file = fopen(file_path, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        uint8_t *buffer = (uint8_t *)malloc(file_size);
        if (buffer) {
            size_t bytes_read = fread(buffer, 1, file_size, file);
            if (bytes_read == file_size) {
                // 加载模块
                char error_buf[128];
                module = wasm_runtime_load(buffer, file_size, error_buf, sizeof(error_buf));
                if (module) {
                    // 实例化模块
                    module_inst = wasm_runtime_instantiate(module, stack_size, 0, error_buf, sizeof(error_buf));
                    if (module_inst) {
                        // 创建执行环境
                    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
                    if (!exec_env) {
                        printf("Failed to create execution environment\n");
                        wasm_runtime_deinstantiate(module_inst);
                        module_inst = NULL;
                        goto cleanup;
                    }
                        // 为参数和结果分配内存并准备调用
                        void* params_native_addr = nullptr;
                        void* results_native_addr = nullptr;
                        uint32_t argv[2] = {0, 0};
                        
                        // 为参数分配内存
                        if (param_size > 0 && params) {
                            if (wasm_runtime_module_malloc(module_inst, param_size, &params_native_addr)) {
                                memcpy(params_native_addr, params, param_size);
                                argv[0] = (uint32_t)((char*)params_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
                            } else {
                                printf("Failed to allocate memory for params\n");
                            }
                        }
                        
                        // 为结果分配内存
                        if (result_size > 0) {
                            if (wasm_runtime_module_malloc(module_inst, result_size, &results_native_addr)) {
                                argv[1] = (uint32_t)((char*)results_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
                            } else {
                                printf("Failed to allocate memory for results\n");
                                if (params_native_addr) {
                                    wasm_runtime_module_free(module_inst, argv[0]);
                                }
                            }
                        }
                        
                        // 查找函数
                        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, function_name);
                        if (func) {
                            // 调用函数
                            if (wasm_runtime_call_wasm(exec_env, func, 2, argv)) {
                                result = true;
                                // 复制结果
                                if (result_size > 0 && results && results_native_addr) {
                                    memcpy(results, results_native_addr, result_size);
                                }
                            } else {
                                printf("Failed to call function: %s\n", wasm_runtime_get_exception(module_inst));
                            }
                        } else {
                            printf("Failed to find function: %s\n", function_name);
                        }
                        
                        // 释放内存
                        if (params_native_addr) {
                            wasm_runtime_module_free(module_inst, argv[0]);
                        }
                        if (results_native_addr) {
                            wasm_runtime_module_free(module_inst, argv[1]);
                        }
                    } else {
                        printf("Failed to instantiate module: %s\n", error_buf);
                    }
                } else {
                    printf("Failed to load module: %s\n", error_buf);
                }
            }
            free(buffer);
        }
        fclose(file);
    } else {
        printf("Failed to open file: %s\n", file_path);
    }
    
cleanup:
      // 清理资源
      if (exec_env) {
          wasm_runtime_destroy_exec_env(exec_env);
      }
      if (module_inst) {
          wasm_runtime_deinstantiate(module_inst);
      }
      if (module) {
          wasm_runtime_unload(module);
      }
      
      wasm_runtime_destroy();
      return result;
}



inline bool execute_wasm_function(const char *wasm_file, const char *function_name,
                          void *params, void *results, int32_t param_size, int32_t result_size,
                          int32_t additional_param1, int32_t additional_param2) {
    // 初始化WAMR运行时
    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));
    init_args.mem_alloc_type = Alloc_With_System_Allocator;
    
    if (!wasm_runtime_full_init(&init_args)) {
        printf("Failed to initialize WAMR runtime\n");
        return false;
    }
    
    bool result = false;
    
    // 尝试加载WASM模块
    uint32_t stack_size = 64 * 1024;  // 64KB栈大小
    wasm_module_t module = NULL;
    wasm_module_inst_t module_inst = NULL;
    wasm_exec_env_t exec_env = NULL;
    
    // 获取WASM文件路径
    const char *file_path = getWASMFilename(wasm_file);
    
    // 加载WASM模块
    FILE *file = fopen(file_path, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long file_size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        uint8_t *buffer = (uint8_t *)malloc(file_size);
        if (buffer) {
            size_t bytes_read = fread(buffer, 1, file_size, file);
            if (bytes_read == file_size) {
                // 加载模块
                char error_buf[128];
                module = wasm_runtime_load(buffer, file_size, error_buf, sizeof(error_buf));
                if (module) {
                    // 实例化模块
                    module_inst = wasm_runtime_instantiate(module, stack_size, 0, error_buf, sizeof(error_buf));
                    if (module_inst) {
                        // 创建执行环境
                        exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
                        if (!exec_env) {
                            printf("Failed to create execution environment\n");
                            wasm_runtime_deinstantiate(module_inst);
                            module_inst = NULL;
                            goto cleanup;
                        }
                        
                        // 为参数和结果分配内存并准备调用
                        void* params_native_addr = nullptr;
                        void* results_native_addr = nullptr;
                        uint32_t argv[4] = {0, 0, static_cast<uint32_t>(additional_param1), static_cast<uint32_t>(additional_param2)};
                        
                        // 为参数分配内存
                        if (param_size > 0 && params) {
                            if (wasm_runtime_module_malloc(module_inst, param_size, &params_native_addr)) {
                                memcpy(params_native_addr, params, param_size);
                                argv[0] = (uint32_t)((char*)params_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
                            } else {
                                printf("Failed to allocate memory for params\n");
                            }
                        }
                        
                        // 为结果分配内存
                        if (result_size > 0) {
                            if (wasm_runtime_module_malloc(module_inst, result_size, &results_native_addr)) {
                                argv[1] = (uint32_t)((char*)results_native_addr - (char*)wasm_runtime_addr_app_to_native(module_inst, 0));
                            } else {
                                printf("Failed to allocate memory for results\n");
                                if (params_native_addr) {
                                    wasm_runtime_module_free(module_inst, argv[0]);
                                }
                            }
                        }
                        
                        // 查找函数
                        wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, function_name);
                        if (func) {
                            // 调用函数
                            if (wasm_runtime_call_wasm(exec_env, func, 4, argv)) {
                                result = true;
                                // 复制结果
                                if (result_size > 0 && results && results_native_addr) {
                                    memcpy(results, results_native_addr, result_size);
                                }
                            } else {
                                printf("Failed to call function: %s\n", wasm_runtime_get_exception(module_inst));
                            }
                        } else {
                            printf("Failed to find function: %s\n", function_name);
                        }
                        
                        // 释放内存
                        if (params_native_addr) {
                            wasm_runtime_module_free(module_inst, argv[0]);
                        }
                        if (results_native_addr) {
                            wasm_runtime_module_free(module_inst, argv[1]);
                        }
                    } else {
                        printf("Failed to instantiate module: %s\n", error_buf);
                    }
                } else {
                    printf("Failed to load module: %s\n", error_buf);
                }
            }
            free(buffer);
        }
        fclose(file);
    } else {
        printf("Failed to open file: %s\n", file_path);
    }
    
cleanup:
    // 清理资源
    if (exec_env) {
        wasm_runtime_destroy_exec_env(exec_env);
    }
    if (module_inst) {
        wasm_runtime_deinstantiate(module_inst);
    }
    if (module) {
        wasm_runtime_unload(module);
    }
    
    wasm_runtime_destroy();
    return result;
}






// 获取WASM文件名的辅助函数
inline const char* getWASMFilename(const char* wasm_file) {
    // 使用更大的缓冲区以避免溢出
    static char full_path[2048];
    char cwd[2048];
    
    // 检查输入参数
    if (!wasm_file || strlen(wasm_file) == 0) {
        printf("getWASMFilename: Empty wasm_file parameter\n");
        return NULL;
    }
    
    printf("getWASMFilename: Looking for WASM file: %s\n", wasm_file);
    
    // 首先检查wasm-apps目录，这是CMakeLists.txt中指定的WASM文件位置
    snprintf(full_path, sizeof(full_path), "/home/chengnie/works/wasm-micro-runtime/tests/unit/simd/wasm-apps/%s", wasm_file);
    if (access(full_path, F_OK) == 0) {
        printf("getWASMFilename: Found WASM file at: %s\n", full_path);
        return full_path;
    }
    
    // 检查构建目录，WASM文件会被复制到这里
    snprintf(full_path, sizeof(full_path), "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/simd/%s", wasm_file);
    if (access(full_path, F_OK) == 0) {
        printf("getWASMFilename: Found WASM file at: %s\n", full_path);
        return full_path;
    }
    
    // 获取当前工作目录
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("getWASMFilename: Current working directory: %s\n", cwd);
        
        // 已知的WASM文件可能存在的位置
        const char* known_locations[] = {
            // 直接检查simd测试目录
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/simd/simd_int_arith_test.wasm",
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/simd/simd_int_arith_test.wasm",
            
            // enhanced_unit_test目录
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/enhanced_unit_test/compilation/simd_int_arith_test.wasm",
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/enhanced_unit_test/compilation/wasm-apps/simd_int_arith_test.wasm",
            
            // 通用simd测试文件
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/simd/simd_test.wasm",
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/enhanced_unit_test/compilation/simd_test.wasm"
        };
        
        // 首先检查已知的文件位置
        for (size_t i = 0; i < sizeof(known_locations)/sizeof(known_locations[0]); i++) {
            printf("getWASMFilename: Checking location: %s\n", known_locations[i]);
            if (access(known_locations[i], F_OK) == 0) {
                printf("getWASMFilename: Found WASM file at: %s\n", known_locations[i]);
                return known_locations[i];
            }
        }
        printf("getWASMFilename: No known location contains the WASM file\n");
        
        // 检查是否需要编译WASM文件
        printf("getWASMFilename: Please ensure the WASM file is compiled\n");
        
        // 定义更多可能的路径模式
        const char* path_patterns[] = {
            // 直接使用文件名（绝对路径或当前目录）
            "%s",                         // 原始文件名
            "./%s",                       // 当前目录
            
            // 标准simd目录结构
            "./simd/%s",                  // 当前目录下的simd文件夹
            "./build/simd/%s",            // 当前目录下的build/simd文件夹
            
            // enhanced_unit_test目录相关路径
            "./build/enhanced_unit_test/compilation/%s",
            "./enhanced_unit_test/compilation/wasm-apps/%s",
            "../build/enhanced_unit_test/compilation/%s",
            "../enhanced_unit_test/compilation/wasm-apps/%s",
            
            // 向上搜索的目录结构
            "../simd/%s",                 // 上级目录下的simd文件夹
            "../build/simd/%s",           // 上级目录下的build/simd文件夹
            "../../simd/%s",              // 上上级目录下的simd文件夹
            "../../build/simd/%s",         // 上上级目录下的build/simd文件夹
            "../../enhanced_unit_test/compilation/wasm-apps/%s",
            "../../build/enhanced_unit_test/compilation/%s",
            
            // 测试目录相关路径
            "../tests/unit/simd/%s",      // 上级目录的tests/unit/simd
            "../tests/unit/enhanced_unit_test/compilation/wasm-apps/%s",
            "../../tests/unit/simd/%s",   // 上上级目录的tests/unit/simd
            "../../tests/unit/enhanced_unit_test/compilation/wasm-apps/%s",
            "./tests/unit/simd/%s",       // 当前目录的tests/unit/simd
            "./tests/unit/enhanced_unit_test/compilation/wasm-apps/%s",
            "./tests/unit/build/simd/%s", // 当前目录的tests/unit/build/simd
            "./tests/unit/build/enhanced_unit_test/compilation/%s",
            
            // 构建目录变体
            "./build/%s",                 // 直接在build目录下
            
            // 基于当前工作目录的路径
            "%s/%s",                      // 当前工作目录直接拼接
            "%s/simd/%s",                 // 当前工作目录下的simd文件夹
            "%s/build/simd/%s",           // 当前工作目录下的build/simd文件夹
            "%s/enhanced_unit_test/compilation/wasm-apps/%s",
            "%s/build/enhanced_unit_test/compilation/%s",
            "%s/../simd/%s",              // 上级目录下的simd文件夹
            "%s/../build/simd/%s",        // 上级目录下的build/simd文件夹
            "%s/../enhanced_unit_test/compilation/wasm-apps/%s",
            
            // 绝对路径 - 项目根目录相关
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/simd/%s",  // 绝对路径1
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/simd/%s", // 绝对路径2
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/enhanced_unit_test/compilation/wasm-apps/%s",
            "/home/chengnie/works/wasm-micro-runtime/tests/unit/build/enhanced_unit_test/compilation/%s"
        };
        
        // 首先尝试不附加CWD的简单路径模式
        for (size_t i = 0; i < 10; i++) { // 前10个模式是不依赖CWD的
            int ret = snprintf(full_path, sizeof(full_path), path_patterns[i], wasm_file);
            if (ret > 0 && ret < sizeof(full_path) && access(full_path, F_OK) == 0) {
                return full_path;
            }
        }
        
        // 然后尝试需要CWD的路径模式
        for (size_t i = 10; i < sizeof(path_patterns)/sizeof(path_patterns[0]); i++) {
            int ret = snprintf(full_path, sizeof(full_path), path_patterns[i], cwd, wasm_file);
            if (ret > 0 && ret < sizeof(full_path) && access(full_path, F_OK) == 0) {
                return full_path;
            }
        }
    }
    
    // 最后尝试直接使用原始文件名
    if (access(wasm_file, F_OK) == 0) {
        return wasm_file;
    }
    
    // 如果所有路径都失败，返回原始文件名
    return wasm_file;
}

#endif // SIMD_TEST_HELPER_H