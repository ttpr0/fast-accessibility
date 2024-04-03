import os
from conan.tools.files import copy

def deploy(graph, dst, **kwargs):
    for name, dep in graph.root.conanfile.dependencies.items():
        src = os.path.join(str(dep.folders.package_folder), "bin")
        copy(dep, "*.so*", src, dst)
        copy(dep, "*.dll", src, dst)
