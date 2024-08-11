# DirTree
This is a tiny C++ command-line utility that generates a comprehensive directory tree in HTML format. It might be useful when you need to back up the data into multiple hard drives and want to save a snapshot of each drive's contents without having to plug them in every time afterward. The generated HTML file visually represents the directory structure and it helps navigate and review the contents of a drive or directory.
## Compile from source
```bash
# 1. Clone
git clone https://github.com/HaojiaWu/DirTree.git
cd DirTree
# 2. Compile
g++ -std=c++17 -o DirTree DirTree.cpp
# 3. Move to bin
sudo mv DirTree /usr/local/bin/
```
## Usage
```bash
DirTree /path/to/directory [-o DirTree.html]
```
Example command line
```bash
DirTree /media/external_drive -o external_drive_structure.html
```
## Output
Below is a snapshot of an example html output.

<img src="https://github.com/HaojiaWu/DirTree/blob/main/example.png" width="600"> <br>
