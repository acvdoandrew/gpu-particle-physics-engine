# GPU-Accelerated Particle Physics Engine

A high-performance particle simulation engine built from scratch in C++ with CUDA acceleration. This project demonstrates low-level optimization techniques and GPU programming for real-time physics simulation.

## 🎯 Project Goals

- Simulate 100,000+ particles at 60 FPS
- Implement fluid dynamics using SPH (Smoothed Particle Hydrodynamics)
- Showcase modern C++ and CUDA optimization techniques
- Build without external physics libraries (from scratch)

## 📊 Current Progress

- [ ] Week 1-4: C++ Fundamentals
- [ ] Week 5-8: Object-Oriented Particle System
- [ ] Week 9-12: Collision Detection & Spatial Partitioning
- [ ] Week 13-16: Basic CUDA Implementation
- [ ] Week 17-20: GPU Optimization
- [ ] Week 21-24: Fluid Simulation

## 🚀 Features

- **CPU Implementation**
  - Verlet integration
  - Spatial hashing for O(n) collision detection
  - Multiple force types (gravity, springs, drag)
- **GPU Implementation** (Coming Soon)
  - CUDA kernels for parallel particle updates
  - Shared memory optimization
  - GPU-based spatial partitioning

## 🛠️ Build Instructions

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16+
- CUDA Toolkit 11.0+ (for GPU version)
- OpenGL 3.3+ (for visualization)

### Building

````bash
mkdir build && cd build
cmake ..
make -j$(nproc)

### Running Examples
```bash
./bin/basic_gravity     # Simple gravity simulation
./bin/collision_demo    # Particle collisions
./bin/benchmark         # Performance testing
````

## 📈 Performance

| Particle Count | CPU (FPS) | GPU (FPS) | Speedup |
| -------------- | --------- | --------- | ------- |
| 1,000          | TBD       | TBD       | TBD     |
| 10,000         | TBD       | TBD       | TBD     |
| 100,000        | TBD       | TBD       | TBD     |

## 🏗️ Architecture

```
┌─────────────────┐     ┌─────────────────┐
│  Particle System │     │   Force System  │
│  ┌───────────┐  │     │  ┌───────────┐  │
│  │ Particles │  │────▶│  │  Gravity  │  │
│  └───────────┘  │     │  └───────────┘  │
│  ┌───────────┐  │     │  ┌───────────┐  │
│  │ Integrator│  │     │  │  Springs  │  │
│  └───────────┘  │     │  └───────────┘  │
└─────────────────┘     └─────────────────┘
         │                       │
         ▼                       ▼
┌─────────────────┐     ┌─────────────────┐
│ Collision System│     │    Renderer     │
│  ┌───────────┐  │     │  ┌───────────┐  │
│  │Spatial Grid│ │     │  │  OpenGL   │  │
│  └───────────┘  │     │  └───────────┘  │
└─────────────────┘     └─────────────────┘
```

## 📚 Learning Resources

- [LearnCpp.com](https://www.learncpp.com/) - C++ fundamentals
- [NVIDIA CUDA Programming Guide](https://docs.nvidia.com/cuda/)
- [Real-Time Collision Detection](http://realtimecollisiondetection.net/) by Christer Ericson

## 🤝 Contributing

This is a personal learning project, but feedback and suggestions are welcome!

## 📝 License

MIT License - See LICENSE file for details

---

**Currently Learning:** C++ Chapter 2.4 - Fundamental Data Types  
**Next Milestone:** Complete basic physics calculator (Week 1)
