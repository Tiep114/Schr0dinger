# Introduction
This is a hobby project intended to build intuition on numerical PDE solvers and to visualize their results.
Furthermore, its goal is to study what is the quality of code produced by LLMs for well-known physical problems,
how well can AI interpret results, and ultimately, how reliable is the computational software developed using these
methods.

The underlying philosophy is to approach software engineering as an empirical science. AI tools combined with domain knowledge
enable a fast pace of code development for engineering and scientific tasks. Evidently, this approach to development has a major
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

## Multi-Compiler Support
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

### 3. Install C++ Compiler

Choose **one** of the following:

**Option A: MSVC (Visual Studio)** - Recommended for Windows
- Download: https://visualstudio.microsoft.com/community/
- Install Visual Studio Community 2026
- During installation, select "Desktop development with C++"
- Add to PATH automatically

**Option B: GCC/MinGW** - Cross-platform alternative
- Download: https://www.mingw-w64.org/
- Extract and add to system PATH
- Verify: `g++ --version`

**Option C: Clang** - LLVM toolchain
- Download: https://releases.llvm.org/
- Install and add to system PATH
- Verify: `clang++ --version`

### 4. Install Python Dependencies (for visualization)
```powershell
pip install matplotlib numpy
```

## Quick Start

The fastest way to get running:

```powershell
# 1. Build and run matrix solver tests
python build.py all test_matrix_solver

# 2. Build and run electrostatic solver
python build.py all test_electrostatic

# 3. Visualize results
python visualize_electrostatic.py
```

That's it! The script handles compiler detection automatically.

## Building the Project

### Option 1: Interactive Mode (Easiest)
```powershell
python build.py
```
- Automatically detects your installed compilers
- Shows a menu to select target (matrix_solver, electrostatic, or both)
- Builds and runs your selection

### Option 2: Command-Line Mode (Fastest)
```powershell
python build.py all test_matrix_solver      # Test matrix solver
python build.py all test_electrostatic      # Test electrostatic solver
python build.py build test_electrostatic    # Build only (no run)
python build.py run test_electrostatic      # Run existing build
```

## Running Tests

### Matrix Solver Tests
Tests 7 examples of linear algebra operations:
- Examples 1-5: Direct solvers (LU, QR, determinant, inverse, eigenvalues)
- Examples 6-7: Iterative solvers (Conjugate Gradient, BiCGSTAB)

```powershell
python build.py all test_matrix_solver
```

### Electrostatic Solver Tests
Simulates a parallel plate capacitor with:
- 25×25 grid spanning 2.5m × 2.5m
- Left plate at 100V, right plate at 0V
- Generates CSV files: potential.csv, Ex.csv, Ey.csv, E_magnitude.csv, energy_density.csv

```powershell
python build.py all test_electrostatic
```

## Visualization

After running the electrostatic test:

```powershell
python visualize_electrostatic.py
```

Generates `electrostatic_solution.png` showing:
1. Potential contours
2. Ex field component
3. Ey field component  
4. Field magnitude |E|
5. Energy density
6. Field vectors overlay

## Troubleshooting

| Problem | Solution |
|---------|----------|
| "Eigen3 not found" | `vcpkg install eigen3:x64-windows` |
| "No compilers found" | Install MSVC, GCC/MinGW, or Clang |
| Visualization fails | `pip install --upgrade matplotlib numpy` |
