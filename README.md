# Introduction
This is a hobby project intended to build intuition on numerical PDE solvers and to visualize their results.
Furthermore, its goal is to study what is the quality of code produced by LLMs for well-known physical problems,
how well can AI interpret results, and ultimately, how reliable the computational software can be using these development
methods.

The underlying philosophy is to approach software engineering as an empirical science. AI tools combined with domain knowledge
enable a fast pace of code production for engineering and scientific tasks. Evidently, this approach to development has a major
challenge; since the majority of the code is produced by AI, the developer has no atomistic understanding of the code. LLMs
are known to produce mistakes and hallucinations, and it is so far an open question what is the level of productivity gains
one can obtain in maintaining and expanding an AI generated codebase, and how does it compare to e.g., a legacy codebase
written by humans and maintained by humans.

I believe that the remedy to the challenge stated above is rigorous unit testing, which explains the choice for the topic of
this project. Numerical investigation of well-known physics enables us to write tests based on immutable physical results.
Multimodal AI has the ability to interpret graphical and numerical scientific data. This can be utilized in constructing
an iterative feedback mechanism for validating your computational software. The objective is to build a stable foundation
based on physics for an increasing hierarchy of complexity.

# How to use

- **Multi-Compiler Support**
  - MSVC (Visual Studio)
  - GCC/MinGW
  - Clang

## Requirements

### System Requirements
- Windows 10 or later
- One of the following C++ compilers:
  - MSVC (Visual Studio Community 2026 or later)
  - GCC/MinGW-w64
  - Clang/LLVM

### Dependencies
- **Eigen3** (3.4.1 or later) - Linear algebra library
- **Python 3.x** (for visualization and build system)
- **matplotlib** (Python, for visualization)
- **numpy** (Python, for data processing)

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/Tiep114/Schr0dinger.git
cd Schr0dinger
```

### 2. Install Eigen3 (if not already installed)

**Using vcpkg (recommended):**
```powershell
vcpkg install eigen3:x64-windows
```
