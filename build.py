#!/usr/bin/env python3
"""
Build script for MatrixSolver and ElectrostaticSolver projects
Supports multiple C++ compilers on Windows: MSVC, GCC/MinGW, Clang
"""

import os
import subprocess
import sys
from pathlib import Path
import shutil

# Compiler configurations
COMPILERS = {
    'msvc': {
        'name': 'MSVC (Visual Studio)',
        'detect': lambda: Path("C:/Program Files/Microsoft Visual Studio/18/Community").exists(),
        'check_cmd': 'cl /?'
    },
    'gcc': {
        'name': 'GCC/MinGW',
        'detect': lambda: shutil.which('g++') is not None,
        'check_cmd': 'g++ --version'
    },
    'clang': {
        'name': 'Clang',
        'detect': lambda: shutil.which('clang++') is not None,
        'check_cmd': 'clang++ --version'
    }
}

def get_eigen_include():
    """Get Eigen3 include path from vcpkg"""
    vcpkg_dir = Path(__file__).parent / 'vcpkg'
    eigen_include = vcpkg_dir / 'installed' / 'x64-windows' / 'include'
    
    if eigen_include.exists():
        print(f"✓ Eigen3 found at: {eigen_include}")
        return str(eigen_include)
    
    return None

def detect_available_compilers():
    """Detect which compilers are available on the system"""
    available = {}
    for compiler_id, compiler_info in COMPILERS.items():
        if compiler_info['detect']():
            available[compiler_id] = compiler_info
    return available

def select_compiler(available_compilers):
    """Allow user to select a compiler"""
    if not available_compilers:
        print("❌ No C++ compilers found on system!")
        print("\nInstall one of the following:")
        print("  - MSVC: Visual Studio Community 2026 with C++ support")
        print("  - GCC: MinGW-w64 (https://www.mingw-w64.org/)")
        print("  - Clang: LLVM (https://releases.llvm.org/)")
        return None
    
    if len(available_compilers) == 1:
        compiler_id = list(available_compilers.keys())[0]
        print(f"✓ Found compiler: {available_compilers[compiler_id]['name']}")
        return compiler_id
    
    # Multiple compilers available - let user choose
    print("\nAvailable compilers:")
    compiler_list = list(available_compilers.items())
    for i, (compiler_id, compiler_info) in enumerate(compiler_list, 1):
        print(f"  {i}. {compiler_info['name']}")
    
    while True:
        choice = input(f"\nSelect compiler (default: 1): ").strip() or "1"
        try:
            idx = int(choice) - 1
            if 0 <= idx < len(compiler_list):
                return compiler_list[idx][0]
        except ValueError:
            pass
        print("Invalid choice, please try again.")
    
    return None

def build_with_msvc(target, source_files, eigen_include, project_dir):
    """Build using MSVC compiler"""
    vs_path = Path("C:/Program Files/Microsoft Visual Studio/18/Community")
    vcvars = vs_path / "VC" / "Auxiliary" / "Build" / "vcvars64.bat"
    
    # Build source file list for compiler
    source_list = " ".join(source_files)
    exe_name = target.replace('.cpp', '.exe')
    
    # Build command using cmd.exe to ensure vcvars is applied
    cmd = f'''cmd /c "call "{vcvars}" >nul 2>&1 && cd /d "{project_dir}" && cl /std:c++latest /EHsc /I"{eigen_include}" {source_list} /Fe:{exe_name}"'''
    
    print(f"\n📦 Building: {exe_name}")
    print(f"   Sources: {', '.join(source_files)}\n")
    
    try:
        result = subprocess.run(cmd, shell=True, capture_output=False, text=True)
        
        if result.returncode == 0:
            print(f"\n✓ Build successful: {exe_name}")
            return True
        else:
            print(f"\n❌ Build failed with error code {result.returncode}")
            return False
    except Exception as e:
        print(f"\n❌ Build error: {e}")
        return False

def build_with_gcc(target, source_files, eigen_include, project_dir):
    """Build using GCC/MinGW compiler"""
    source_list = " ".join(source_files)
    exe_name = target.replace('.cpp', '.exe')
    
    # GCC command
    cmd = f'g++ -std=c++17 -Wall -Wextra -I"{eigen_include}" {source_list} -o {exe_name}'
    
    print(f"\n📦 Building with GCC: {exe_name}")
    print(f"   Sources: {', '.join(source_files)}\n")
    
    try:
        result = subprocess.run(cmd, shell=True, cwd=project_dir, capture_output=False, text=True)
        
        if result.returncode == 0:
            print(f"\n✓ Build successful: {exe_name}")
            return True
        else:
            print(f"\n❌ Build failed with error code {result.returncode}")
            return False
    except Exception as e:
        print(f"\n❌ Build error: {e}")
        return False

def build_with_clang(target, source_files, eigen_include, project_dir):
    """Build using Clang compiler"""
    source_list = " ".join(source_files)
    exe_name = target.replace('.cpp', '.exe')
    
    # Clang command
    cmd = f'clang++ -std=c++17 -Wall -Wextra -I"{eigen_include}" {source_list} -o {exe_name}'
    
    print(f"\n📦 Building with Clang: {exe_name}")
    print(f"   Sources: {', '.join(source_files)}\n")
    
    try:
        result = subprocess.run(cmd, shell=True, cwd=project_dir, capture_output=False, text=True)
        
        if result.returncode == 0:
            print(f"\n✓ Build successful: {exe_name}")
            return True
        else:
            print(f"\n❌ Build failed with error code {result.returncode}")
            return False
    except Exception as e:
        print(f"\n❌ Build error: {e}")
        return False

def build(compiler, target, source_files, eigen_include, project_dir):
    """Build with selected compiler"""
    builders = {
        'msvc': build_with_msvc,
        'gcc': build_with_gcc,
        'clang': build_with_clang
    }
    
    builder = builders.get(compiler)
    if not builder:
        print(f"❌ Unknown compiler: {compiler}")
        return False
    
    return builder(target, source_files, eigen_include, project_dir)

def run_executable(exe_name, project_dir):
    """Run the compiled executable"""
    exe_path = Path(project_dir) / exe_name
    
    if not exe_path.exists():
        print(f"❌ Executable not found: {exe_path}")
        return False
    
    print(f"\n{'='*60}")
    print(f"Running: {exe_name}")
    print(f"{'='*60}\n")
    
    try:
        subprocess.run([str(exe_path)], cwd=project_dir, check=True)
        return True
    except subprocess.CalledProcessError:
        print(f"\n❌ Execution failed!")
        return False
    except Exception as e:
        print(f"\n❌ Error running executable: {e}")
        return False

def main():
    print("=" * 60)
    print("MatrixSolver & ElectrostaticSolver Build Script")
    print("=" * 60)
    print()
    
    project_dir = Path(__file__).parent
    
    # Get Eigen include path
    eigen_include = get_eigen_include()
    
    if not eigen_include:
        print("❌ Eigen3 not found!")
        print("Install it with: vcpkg install eigen3:x64-windows")
        return 1
    
    # Detect and select compiler
    available_compilers = detect_available_compilers()
    compiler = select_compiler(available_compilers)
    
    if not compiler:
        return 1
    
    targets = {
        'test_matrix_solver': {
            'exe': 'test_matrix_solver.exe',
            'sources': ['test_matrix_solver.cpp', 'MatrixSolver.cpp']
        },
        'test_electrostatic': {
            'exe': 'test_electrostatic.exe',
            'sources': ['test_electrostatic.cpp', 'ElectrostaticSolver.cpp', 'MatrixSolver.cpp']
        }
    }
    
    # Parse command-line arguments
    if len(sys.argv) > 1:
        command = sys.argv[1].lower()
        target = sys.argv[2].lower() if len(sys.argv) > 2 else 'all'
    else:
        # Interactive mode
        print("\nUsage: python build.py [command] [target]")
        print("\nCommands:")
        print("  build [target]  - Build only (no execution)")
        print("  run [target]    - Run only (no rebuild)")
        print("  all [target]    - Build and run (default)")
        print("\nTargets:")
        print("  test_matrix_solver   - Matrix solver test")
        print("  test_electrostatic   - Electrostatic solver test")
        print("  all                  - Build/run both (default)")
        print()
        
        print("Available targets:")
        print("  1. test_matrix_solver   - Basic matrix operations test")
        print("  2. test_electrostatic   - FDM electrostatic solver test")
        print("  3. all                  - Build both")
        print()
        
        choice = input("Select target to build (default: all): ").strip().lower() or "all"
        
        # Map numeric choices to target names
        choice_map = {
            '1': 'test_matrix_solver',
            '2': 'test_electrostatic',
            '3': 'all'
        }
        
        target = choice_map.get(choice, choice)
        command = 'all'
    
    # Validate target
    if target == 'all':
        build_list = list(targets.keys())
    elif target in targets:
        build_list = [target]
    else:
        print(f"❌ Invalid target: {target}")
        return 1
    
    # Execute command
    if command == 'build':
        # Build only
        for target_name in build_list:
            target_info = targets[target_name]
            print(f"\nTarget: {target_name}")
            if not build(compiler, target_name, target_info['sources'], eigen_include, project_dir):
                return 1
        print(f"\n✓ Build completed!")
        return 0
    
    elif command == 'run':
        # Run only
        for target_name in build_list:
            target_info = targets[target_name]
            print(f"\nTarget: {target_name}")
            if not run_executable(target_info['exe'], project_dir):
                return 1
        print(f"\n✓ Execution completed!")
        return 0
    
    elif command == 'all':
        # Build and run
        for target_name in build_list:
            target_info = targets[target_name]
            
            print(f"\n{'='*60}")
            print(f"Target: {target_name}")
            print(f"{'='*60}")
            
            # Build
            if not build(compiler, target_name, target_info['sources'], eigen_include, project_dir):
                return 1
            
            # Run
            if not run_executable(target_info['exe'], project_dir):
                return 1
        
        print(f"\n✓ All builds and executions completed successfully!")
        return 0
    
    else:
        print(f"❌ Invalid command: {command}")
        print("Valid commands: build, run, all")
        return 1

if __name__ == '__main__':
    sys.exit(main())
