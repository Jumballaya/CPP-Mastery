# C++ Learning Journey

This repository tracks my structured journey from senior software engineer to expert-level C++ systems programmer — with the goal of becoming a senior or lead developer on a game engine team.

Every system, log, and case study in this repo is built to simulate real-world runtime problems, memory challenges, and architectural decisions common in production-grade engines.

### Objective

To transition into a professional game engine development role by mastering C++ systems programming through structured, project-based learning. This repository serves as both a personal learning archive and a public portfolio for future employers.

---

## Current Focus

Currently building foundational C++ systems focused on ownership, RAII, and memory safety — including custom smart pointers, rollback guards, and lifetime tracking. This forms the base of more advanced systems like allocators and ECS in the coming weeks.

---

## Curriculum Overview

This project follows a 24-week structured curriculum designed to prepare me for professional roles in game engine and runtime systems development.

The curriculum is project-driven and focuses on core systems topics such as:

- Custom memory allocators and arena systems
- Entity-Component-System (ECS) architecture
- Job systems, fibers, and task graph orchestration
- Templates, traits, and compile-time architecture
- Static analysis, profiling, and diagnostics
- Modular system design and plugin architecture

### Technologies & Tooling

- C++17/20/23, CMake, g++, clang++
- Debugging: `gdb`, sanitizers, logging systems
- Profiling: `Tracy`, `Valgrind`, `perf`
- Static analysis: `clang-tidy`, `cppcheck`
- Testing: `doctest`, `Catch2`, `GoogleTest`
- IDE: VS Code + WSL

### Key Systems I’ll Be Building

- 🔸 `ResourceManager<T>` — Ownership, RAII, and rollback safety
- 🔸 `ECS Runtime` — SoA data layout, component iteration, system scheduler
- 🔸 `Job System` — Thread pool, lock-free queues, task graph with fibers
- 🔸 `Custom Allocator` — Arena allocator with diagnostics
- 🔸 `Instrumentation Layer` — In-engine profiling with Tracy
- 🔸 `Plugin Runtime` — Modular loadable systems and hot-reloadable symbols

---

## Weekly logging

Each week, I document my learning and reflections in the `logs/` folder as `logs/week-XX.md`. These logs reinforce retention and track my engineering evolution.

Each log answers:

- What did I learn?
- What was the most difficult thing to learn?
- How does this help me get to lead C++ developer?

---

## Monthly Case Studies

At the end of each month, I write a deep case study on that month’s core system, stored in the `case-studies/` folder as `case-XX-topic.md`.

Each study includes:

- Design overview
- Architecture decisions
- Debugging stories
- Tradeoffs
- Lessons learned

These serve as both engineering retrospectives and portfolio artifacts.
