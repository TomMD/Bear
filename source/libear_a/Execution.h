/*  Copyright (C) 2012-2017 by László Nagy
    This file is part of Bear.

    Bear is a tool to generate compilation database for clang tooling.

    Bear is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bear is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config.h"

#include <unistd.h>
#if defined HAVE_SPAWN_HEADER
# include <spawn.h>
#endif

#include "libear_a/State.h"
#include "libear_a/SessionSerializer.h"
#include "libear_a/Result.h"
#include "libear_a/Resolver.h"

namespace ear {

    constexpr char command_separator[] = "--";
    constexpr char file_flag[] = "-f";
    constexpr char search_flag[] = "-s";

    struct Execution_Z {
        const char **argv_;
        const char **envp_;

        virtual ~Execution_Z() noexcept = default;
    };

    struct Execve_Z : public Execution_Z {
        const char *path_;
    };

    struct Execvpe_Z : public Execution_Z {
        const char *file_;
    };

    struct ExecvP_Z : public Execution_Z {
        const char *file_;
        const char *search_path_;
    };

    struct ExecutionWithoutFork_Z : public Execution_Z {
        pid_t *pid_;
        const posix_spawn_file_actions_t *file_actions_;
        const posix_spawnattr_t *attrp_;
    };

    struct Spawn_Z : public ExecutionWithoutFork_Z {
        const char *path_;
    };

    struct Spawnp_Z : public ExecutionWithoutFork_Z {
        const char *file_;
    };


    class Execution {
    public:
        Result<int> apply(Resolver const &linker, State const *state) noexcept;

    protected:
        virtual Result<int> apply(Resolver const &) noexcept = 0;

        virtual Result<int> apply(Resolver const &, Serializable const &) noexcept = 0;
    };


    class Execve : public Execution {
    public:
        Execve(const char *path, char *const argv[], char *const envp[]) noexcept;

    protected:
        Result<int> apply(Resolver const &linker) noexcept override;

        Result<int> apply(Resolver const &linker, Serializable const &session) noexcept override;

    private:
        const char *path_;
        const char **argv_;
        const char **envp_;
    };

    class Execvpe : public Execution {
    public:
        Execvpe(const char *file, char *const argv[], char *const envp[]) noexcept;

    protected:
        Result<int> apply(Resolver const &linker) noexcept override;

        Result<int> apply(Resolver const &linker, Serializable const &session) noexcept override;

    private:
        const char *file_;
        const char **argv_;
        const char **envp_;
    };

    class ExecvP : public Execution {
    public:
        ExecvP(const char *file, const char *search_path, char *const argv[], char *const envp[]) noexcept;

    protected:
        Result<int> apply(Resolver const &linker) noexcept override;

        Result<int> apply(Resolver const &linker, Serializable const &session) noexcept override;

    private:
        const char *file_;
        const char *search_path_;
        const char **argv_;
        const char **envp_;
    };

    class Spawn : public Execution {
    public:
        Spawn(pid_t *pid,
              const char *path,
              const posix_spawn_file_actions_t *file_actions,
              const posix_spawnattr_t *attrp,
              char *const argv[],
              char *const envp[]) noexcept;

    protected:
        Result<int> apply(Resolver const &linker) noexcept override;

        Result<int> apply(Resolver const &linker, Serializable const &session) noexcept override;

    private:
        pid_t *pid_;
        const char *path_;
        const posix_spawn_file_actions_t *file_actions_;
        const posix_spawnattr_t *attrp_;
        const char **argv_;
        const char **envp_;
    };

    class Spawnp : public Execution {
    public:
        Spawnp(pid_t *pid,
               const char *file,
               const posix_spawn_file_actions_t *file_actions,
               const posix_spawnattr_t *attrp,
               char *const argv[],
               char *const envp[]) noexcept;

    protected:
        Result<int> apply(Resolver const &linker) noexcept override;

        Result<int> apply(Resolver const &linker, Serializable const &session) noexcept override;

    private:
        pid_t *pid_;
        const char *file_;
        const posix_spawn_file_actions_t *file_actions_;
        const posix_spawnattr_t *attrp_;
        const char **argv_;
        const char **envp_;
    };

}
