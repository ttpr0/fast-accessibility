from pathlib import Path
from nanobind_stubgen.NanobindStubsGenerator import NanobindStubsGenerator

def main():
    generator = NanobindStubsGenerator("._pyaccess_ext", "pyaccess")
    stubs = generator.analyse()

    output_path = Path("./pyaccess")
    stubs.export(output_path)


if __name__ == "__main__":
    main()