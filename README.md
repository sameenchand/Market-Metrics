# README
For the best experience, please view this in a Markdown‑compatible reader.

**Date:** April 25, 2025  
**Environment:** Visual Studio Community, Windows 11 24H2

## Instructions
1. Extract the archive with the project files to a folder.
2. Open up Visual Studio and `Create a new project with existing code` using the files extracted in the previous step.
3. Choose `C++` as the programming language and `Console application` as the project type.
4. After project creation, go to project setings and under Configuration Properties and select `vcpkg`. In vcpkg, under `General`, turn on `Use Vcpkg manifest`.
5. Now, create an account on [Stock Data](https://www.stockdata.org/) and get a free API key.
6. Create a text file named `config.txt` in the project directory and write `API_KEY=<whaever your API key is>`

If you followed the above instructions correctly, the program should now compile and run successfully.
