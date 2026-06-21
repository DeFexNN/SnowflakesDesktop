# SnowflakesDesktop

Native desktop application (coursework/simulation) showcasing real-time simulation loops, native UI, and debugging artifacts.

Tech stack

- C++ (MSVC) or C# (.NET) — chosen for native performance and deterministic behavior.
- Visual Studio solution files — integrated debugging and profiling.
- Native UI (Win32/GDI+, WPF, or Qt) depending on implementation path.

Purpose & where to look

- src/ or main project files implementing the simulation loop and UI bindings.
- x64/Debug artifacts indicate active local development and debug symbols.

Interesting code patterns

- Separation of simulation update loop and rendering (fixed timestep or semi-fixed) for stability.
- Model-view separation that keeps physics isolated and testable.
- Use of unit test harnesses for validating invariants (if present).

Build & test

1. Open solution in Visual Studio and build Debug or Release.
2. Run in IDE for debug visualizers and profiling tools.

Interview points

- Choice of timestep integration and its impact on determinism.
- Profiling and micro-optimizations to improve frame-time consistency.