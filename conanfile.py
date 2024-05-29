from conan import ConanFile

class ManuelConan(ConanFile):
    generators = ("CMakeToolchain", "CMakeDeps")
    settings = ("os", "build_type", "arch", "compiler")

    def requirements(self):
        self.requires("fmt/10.2.1")
        self.requires("catch2/3.6.0")

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.25]")

    
    def layout(self):
        self.folders.generators = ""