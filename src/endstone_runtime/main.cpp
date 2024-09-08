// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <exception>
#include <thread>

#include <pybind11/embed.h>
#include <spdlog/spdlog.h>

#include "endstone/detail/devtools/devtools.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/logger_factory.h"

#if __GNUC__
#define ENDSTONE_RUNTIME_CTOR __attribute__((constructor))
#else
#define ENDSTONE_RUNTIME_CTOR
#endif

namespace py = pybind11;

ENDSTONE_RUNTIME_CTOR int main()
{
    spdlog::flush_every(std::chrono::seconds(5));
    auto &logger = endstone::detail::LoggerFactory::getLogger("EndstoneRuntime");
    try {
        logger.info("Initialising...");

        // Initialise an isolated Python environment to avoid installing signal handlers
        // https://docs.python.org/3/c-api/init_config.html#init-isolated-conf
        PyConfig config;
        PyConfig_InitIsolatedConfig(&config);
        config.isolated = 0;
        config.use_environment = 1;
        config.install_signal_handlers = 0;
        py::initialize_interpreter(&config);
        py::module_::import("threading");  // https://github.com/pybind/pybind11/issues/2197
        py::module_::import("numpy");      // https://github.com/numpy/numpy/issues/24833
        py::gil_scoped_release release{};
        release.disarm();

        // Install hooks
        endstone::detail::hook::install();

#ifdef ENDSTONE_DEVTOOLS
        // Create devtools window
        auto thread = std::thread(&endstone::detail::devtools::render);
        thread.detach();
#endif
    }
    catch (const std::exception &e) {
        logger.error("An exception occurred while initialising Endstone runtime.");
        logger.error("{}", e.what());
        throw e;
    }
    auto &logger2 = endstone::detail::LoggerFactory::getLogger("EndstoneLuaRuntime");
    try {
        logger2.info("Initialising...");
        logger2.error("NOT IMPLEMENTED YET");
    }
    catch (const std::exception &e) {
        logger2.error("An exception occurred while initialising Endstone runtime.");
        logger2.error("{}", e.what());
        throw e;
    }

}

#ifdef _WIN32
#include <windows.h>

[[maybe_unused]] BOOL WINAPI DllMain(_In_ HINSTANCE /*module*/,  // handle to DLL module
                                     _In_ DWORD reason,          // reason for calling function
                                     _In_ LPVOID /*reserved*/)   // reserved
{
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        GetConsoleMode(console, &mode);
        SetConsoleMode(console, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        main();
        break;
    }
    case DLL_PROCESS_DETACH: {
        break;
    }
    default:
        break;
    }
    return TRUE;
}
#endif
