# ShOS - A Simple Operating System from Scratch

![ShOS Banner](https://img.shields.io/badge/OS-ShOS-blueviolet)
![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Architecture](https://img.shields.io/badge/Architecture-x86__32bit-red)
![Version](https://img.shields.io/badge/Version-1.0-orange)

ShOS is a complete, educational operating system built from scratch for x86 architecture. It features a custom kernel, shell, filesystem, text editor, and various utilities - all written in C and assembly.

## 🌟 Features

### Core System
- **32-bit Protected Mode Kernel** with basic memory management
- **Multiboot Compliant Bootloader** for compatibility with standard bootloaders
- **VGA Text Mode** (80x25) display driver with full text output capabilities
- **PS/2 Keyboard Driver** with full input handling

### Shell Interface
- **Interactive Command Shell** with 20+ built-in commands
- **Command History** and line editing support
- **Tab Completion** for file and directory names
- **Syntax Highlighting** for better user experience

### File System
- **RAM-based Hierarchical Filesystem** with full directory support
- **File Operations**: create, read, write, delete, edit
- **Directory Operations**: create, list, navigate, remove
- **File Extensions Support** for any file type
- **1KB Maximum File Size** per file

### Utilities & Applications
- **Text Editor** (nano-like) with save/load functionality
- **Tic Tac Toe Game** with full interactive gameplay
- **Mathematical Calculator** with basic operations
- **System Information** display

### Developer Features
- **Custom System Calls** for application development
- **Debug Mode** with system status monitoring
- **Memory Usage Statistics**
- **Process Management** (basic)

## 🛠️ Technology Stack

- **Assembly**: NASM (Netwide Assembler) for bootloader and low-level routines
- **C Language**: GCC for kernel and system components
- **Linker**: GNU LD with custom linker script
- **Emulation**: QEMU for testing and development
- **Architecture**: x86 (32-bit protected mode)

## 📁 Project Structure

```
shos/
├── boot/                 # Bootloader components
│   └── boot.asm         # Multiboot-compliant bootloader
├── kernel/              # Kernel source
│   ├── kernel.c         # Main kernel entry point
│   ├── vga.[ch]         # VGA text mode driver
│   ├── klib.[ch]        # Kernel library utilities
│   └── splash.[ch]      # Boot splash screen
├── fs/                  # Filesystem
│   ├── fs.[ch]          # Filesystem implementation
│   └── editor.[ch]      # Text editor application
├── apps/                # User applications
│   ├── shell.[ch]       # Command shell
│   ├── tictactoe.[ch]   # Tic Tac Toe game
│   └── math.[ch]        # Mathematical functions
├── include/             # Header files
│   ├── system.h         # System definitions
│   └── types.h          # Type definitions
├── linker.ld           # Linker script
├── Makefile            # Build system
└── README.md           # This file
```

## 🚀 Quick Start

### Prerequisites

- **Linux/macOS/Windows (WSL)**
- NASM (`sudo apt install nasm`)
- GCC with multilib support (`sudo apt install gcc-multilib`)
- QEMU (`sudo apt install qemu-system-x86`)
- Make (`sudo apt install make`)

### Installation & Build

```bash
# Clone the repository
git clone https://github.com/yourusername/shos.git
cd shos

# Build the operating system
make clean
make

# Run in QEMU emulator
make run

# Run with debug output
make debug
```

### Manual Build Steps

```bash
# Compile bootloader
nasm -f elf32 -o boot.o boot.asm

# Compile kernel components
gcc -ffreestanding -m32 -Wall -Wextra -c -o kernel.o kernel.c
gcc -ffreestanding -m32 -Wall -Wextra -c -o vga.o vga.c
gcc -ffreestanding -m32 -Wall -Wextra -c -o klib.o klib.c
gcc -ffreestanding -m32 -Wall -Wextra -c -o splash.o splash.c

# Compile filesystem
gcc -ffreestanding -m32 -Wall -Wextra -c -o fs.o fs.c
gcc -ffreestanding -m32 -Wall -Wextra -c -o editor.o editor.c

# Compile applications
gcc -ffreestanding -m32 -Wall -Wextra -c -o shell.o shell.c
gcc -ffreestanding -m32 -Wall -Wextra -c -o tictactoe.o tictactoe.c

# Link all components
ld -T linker.ld -melf_i386 -o shos.bin   boot.o kernel.o vga.o klib.o splash.o   fs.o editor.o shell.o tictactoe.o

# Run the OS
qemu-system-i386 -kernel shos.bin
```

## 🎮 Available Commands

### File System Commands
| Command | Usage | Description |
|---------|-------|-------------|
| `ls` | `ls [path]` | List directory contents |
| `pwd` | `pwd` | Print working directory |
| `cd` | `cd [path]` | Change directory |
| `mkdir` | `mkdir <dirname>` | Create directory |
| `touch` | `touch <filename>` | Create empty file |
| `cat` | `cat <filename>` | Display file contents |
| `edit` | `edit <filename>` | Edit file in text editor |
| `write` | `write <filename> <content>` | Write content to file |
| `rm` | `rm <path>` | Remove file or empty directory |

### System Commands
| Command | Usage | Description |
|---------|-------|-------------|
| `help` | `help` | Show available commands |
| `clear` | `clear` | Clear the screen |
| `echo` | `echo <text>` | Echo text to screen |
| `info` | `info` | Show system information |
| `shutdown` | `shutdown` | Shutdown the system |

### Mathematical Commands
| Command | Usage | Description |
|---------|-------|-------------|
| `add` | `add <num1> <num2>` | Add two numbers |
| `sub` | `sub <num1> <num2>` | Subtract two numbers |
| `mul` | `mul <num1> <num2>` | Multiply two numbers |
| `div` | `div <num1> <num2>` | Divide two numbers |

### Entertainment
| Command | Usage | Description |
|---------|-------|-------------|
| `tictactoe` | `tictactoe` | Play Tic Tac Toe game |

## 🎯 Usage Examples

### File Management
```bash
# Create and navigate directories
mkdir projects
cd projects
mkdir shos
cd shos

# Create and edit files
touch README.txt
edit README.txt
# [Edit file using Ctrl+S to save, Ctrl+X to exit]

# View file contents
cat README.txt

# Write content directly
write hello.txt "Hello, World!"

# Remove files
rm hello.txt
```

### System Operations
```bash
# Get system information
info

# Perform calculations
add 5 3        # Result: 8
mul 6 7        # Result: 42

# Play a game
tictactoe

# Clean up and exit
shutdown
```

## 🔧 Technical Specifications

### Memory Layout
- **Kernel Load Address**: 0x100000 (1MB)
- **Stack Size**: 16KB
- **Heap**: Not implemented (static allocation only)
- **VGA Buffer**: 0xB8000

### Filesystem Limits
- **Max Filename Length**: 32 characters
- **Max Path Length**: 128 characters
- **Max Files**: 100
- **Max Directories**: 50
- **Max File Size**: 1KB

### Hardware Support
- **CPU**: x86-compatible (386+)
- **Memory**: 1MB+ required
- **Display**: VGA-compatible text mode
- **Input**: PS/2 keyboard

## 🐛 Debugging

### QEMU Debug Options
```bash
# Run with debug output
make debug

# Run with serial console
make serial

# Run with GDB support
make gdb
```

### Common Issues
1. **"undefined reference" errors**: Check function signatures in header files
2. **QEMU not starting**: Verify QEMU installation and permissions
3. **Keyboard not working**: Check PS/2 emulation settings

## 📚 Learning Resources

This project is excellent for learning about:
- Operating System fundamentals
- x86 Architecture and protected mode
- Memory management and paging
- Filesystem design and implementation
- Device drivers and hardware interaction
- System calls and interrupt handling

### Recommended Reading
- [OSDev Wiki](https://wiki.osdev.org/)
- "Operating Systems: Three Easy Pieces" by Remzi and Andrea Arpaci-Dusseau
- "x86 Assembly Language and C Fundamentals" by Joseph Cavanagh
- "Modern Operating Systems" by Andrew S. Tanenbaum

## 🤝 Contributing

We welcome contributions! Please see our contributing guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow the existing code style
- Add comments for complex functionality
- Update documentation for new features
- Test thoroughly before submitting

## 📊 Project Status

| Component | Status | Notes |
|-----------|---------|-------|
| Bootloader | ✅ Complete | Multiboot compliant |
| Kernel | ✅ Complete | Basic protected mode |
| VGA Driver | ✅ Complete | Text mode only |
| Keyboard | ✅ Complete | PS/2 support |
| Filesystem | ✅ Complete | RAM-based |
| Shell | ✅ Complete | 20+ commands |
| Text Editor | ✅ Complete | Nano-like |
| Games | ✅ Complete | Tic Tac Toe |
| Networking | ❌ Not started | Future feature |
| Multitasking | ❌ Not started | Future feature |

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **OSDev Community** for invaluable resources and guidance
- **QEMU Developers** for the excellent emulator
- **GNU Toolchain** for the compilation tools
- **All Contributors** who helped shape this project

## 📞 Support

If you have any questions or need help:

- Create an [Issue](https://github.com/yourusername/shos/issues)
- Join our [Discussions](https://github.com/yourusername/shos/discussions)
- Check the [Wiki](https://github.com/yourusername/shos/wiki) for documentation

## 🚀 Future Development

Planned features for future versions:
- [ ] Process management and multitasking
- [ ] Virtual memory and paging
- [ ] FAT32 filesystem support
- [ ] Basic networking stack
- [ ] GUI interface
- [ ] Package management system
- [ ] Scripting support

---

**ShOS** - Because every great journey starts with a simple step into the world of operating systems development! 🐧✨
