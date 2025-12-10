import tempfile
import os

def into_comment(text: str, indentation: int) -> str:
    out = " " * indentation + "/**\n"
    out += "\n".join(" " * indentation + " * " + line for line in text.splitlines())
    out += "\n" + " " * indentation + " */\n"
    return out

def document_file(filename: str, docs: dict[str, str]) -> None:
    with open(filename, "r") as f:
        lines = f.readlines()

    # stored in %temp% on Windows and /tmp on Mac/Linux
    # used as a backup of the original file in case something goes wrong
    temp_file_prefix = f"es_docs_{filename.replace("\\", "_").replace("/", "_")}_"
    with tempfile.NamedTemporaryFile("w", delete=False, prefix=temp_file_prefix) as f:
        for line in lines:
            f.write(line)

    with open(filename, "w") as f:
        in_doc = False
        check_in_doc = False

        for line_number, line in enumerate(lines):
            if check_in_doc:
                in_doc = line.strip().startswith("/*")
                check_in_doc = False

            if not in_doc:
                f.write(line)

            if in_doc and line.strip().endswith("*/"):
                in_doc = False

            if line.strip().startswith("ES_API_DOC(") and line.strip().endswith(")"):

                doc_id = line.strip().removeprefix("ES_API_DOC(").removesuffix(")")
                check_in_doc = True

                # make sure the file keeps getting written to otherwise the original file will be lost
                if doc_id not in docs:
                    print(f"INVALID DOC ID id: {doc_id} ON line: {line_number + 1} IN file: {filename}!")
                    continue

                indentation = len(line) - len(line.lstrip())

                if not line.endswith("\n"):
                    f.write("\n")
                f.write(into_comment(docs[doc_id], indentation))

def get_docs() -> dict[str, str]:
    docs: dict[str, str] = {}
    doc_files = os.listdir("doc/api")

    for file_name in doc_files:
        with open("doc/api/" + file_name, "r") as f:
            docs[file_name[:-4]] = f.read()

    return docs

def main() -> None:
    docs = get_docs()

    document_file("include/EasySockets/EasySockets.hpp", docs)
    document_file("include/EasySockets/Windows/WindowsSocket.hpp", docs)
    document_file("include/EasySockets/Windows/WinsockContext.hpp", docs)
    document_file("include/EasySockets/Posix/PosixSocket.hpp", docs)
    document_file("include/EasySockets/Posix/PosixContext.hpp", docs)

if __name__ == "__main__":
    main()