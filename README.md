# Operator-Console-Qt
This project was built with Qt 5.1.11, and Imatest IT 5.1.
The project will build and run, but not all features are implemented yet.
Image Acqusition was tested using Qt's camera intefrace, using a built-in webcam.

_Note for Linux development_: Currently the Operator Console on Linux must be compiled with QT 5.5 and in order to run, you must use the QT libraries in the MATLAB Runtime folder. Please set your LD_LIBRARY_PATH as follows:

```
export LD_LIBRARY_PATH=/usr/local/MATLAB/MATLAB_Runtime/v93/bin/glnxa64:/usr/local/MATLAB/MATLAB_Runtime/v93/runtime/glnxa64:$LD_LIBRARY_PATH
```

