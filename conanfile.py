import os
from conans import ConanFile, CMake

class PyphysnaConanFile(ConanFile):
    keep_imports = True
    settings = "os", "compiler", "build_type", "arch"
    # comma-separated list of requirements
    requires = \
        "boost/1.76.0"
        #
    generators = "cmake_find_package"


    def build(self):
        cmake = CMake(self) # it will find the packages by using our auto-generated FindXXX.cmake files
        cmake.configure()
        cmake.build()

    def imports(self):
        dest = os.getenv("CONAN_IMPORT_PATH", "lib")
        self.copy("*.dll", dst=dest, src="bin") # From bin to bin
        self.copy("*.a", dst=dest, src="bin") # From bin to bin
        self.copy("*.dylib*", dst=dest, src="library") # From library to bin
        self.copy("*.so*", dst=dest, src="lib") # From library to bin
        # self.copy("*", src="@bindirs", dst="bin")
        # self.copy("*", src="@libdirs", dst="lib")