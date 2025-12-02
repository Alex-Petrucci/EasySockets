import tempfile
import textwrap

DOCS = {
    "context": textwrap.dedent("""\
    An object which is required to exist for es::Sockets to be used.
    This ensures that any underlying setup and cleanup code is called.
    <br>For example:
    <code>
    es::Context context{}; // must be created before any sockets are created
    es::Socket socket{es::IpVersion::ipv4, es::Protocol::tcp};
    socket.bind({"0.0.0.0", 8080});
    </code>
    <br>Can be moved from but not copied.
    """),
    "socket": textwrap.dedent("""\
    An object which represents a connection between two applications.
    <br>On creation, type of ip addresses that the socket will use and the
    layer 4/transport layer protocol is specified. 
    <br>From this, methods like <code>.bind_to(EndPoint end_point)</code> 
    and <code>.connect_to(EndPoint end_point)</code> to give the socket 
    an address and port so that it can start communicating with other applications.
    <br>For connectionless protocols like UDP, data can be sent and received
    without binding or connecting first.
    <br>Can be moved from but not copied.
    """),
    "socket_constructor": textwrap.dedent("""\
    Constructor for Socket.
    @param ip_version The version of ip addresses that the socket will use.
    @param protocol The layer 4/transport layer protocol that the socket will use.
    """),
    "socket_bind_to": textwrap.dedent("""\
    Binds a socket to the given end point.
    @param end_point The endpoint that the socket will use. <code>"0.0.0.0"</code>
    will bind to all IPv4 interfaces, <code>"::0"</code>will bind to all IPv6 interfaces
    and <code>"::"</code>" will bind to all interfaces.
    """),
    "socket_connect_to": textwrap.dedent("""\
    Connects a socket to the given end point.
    @param end_point The endpoint that the socket will use."""),
    "socket_listen_for_connections": textwrap.dedent("""\
    Makes the socket start listening for incoming connections.
    <br>Must be bound with <code>.bind(EndPoint end_point)</code>
    before being called.
    @param backlog The amount of connections that can be waiting
    to be accepted at a time. If the amount of connections exceeds
    the backlog, then new connections will be dropped.
    <br>Connections can be accepted with <code>.accept_connection</code>.
    """),
    "socket_accept_connection": textwrap.dedent("""\
    Accepts incoming connections.
    <br><code>.listen(int backlog)</code>must be called first.
    @return New socket connected to the socket that was attempting
    to connect.
    """),
    "socket_receive_data": textwrap.dedent("""\
    Waits to receive data from the connected end point.
    <br>Must be connected to another end point.
    @param buffer The buffer which data received will be placed into.
    @param buffer_size The size of the buffer provided.
    @return The amount of data in bytes received.
    """),
    "socket_receive_data_from": textwrap.dedent("""\
    Waits to receive data from any endpoint.
    <br>Can only be used with connectionless protocols like UDP.
    @param buffer The buffer which data received will be placed into.
    @param buffer_size The size of the buffer provided.
    @param sender_end_point An out parameter will give back the end point
    of the sender.
    @return The amount of data in bytes received.
    """),
    "socket_send_data": textwrap.dedent("""\
    Sends data to the connected end point.
    <br>Must be connected to another end point.
    @param buffer The buffer which data received will be placed into.
    @param buffer_size The size of the buffer provided.
    @return The amount of data in bytes sent.
    """),
    "socket_send_data_to": textwrap.dedent("""\
    Sends data to the provided end point.
    <br>Must be connected to another end point.
    @param buffer The buffer which data received will be placed into.
    @param buffer_size The size of the buffer provided.
    @param end_point The endpoint that the data will be sent to.
    @return The amount of data in bytes sent.
    """)
}

def into_comment(text: str, indentation: int) -> str:
    out = " " * indentation + "/**\n"
    out += "\n".join(" " * indentation + " * " + line for line in text.splitlines())
    out += "\n" + " " * indentation + " */\n"
    return out

def document_file(filename: str) -> None:
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
                if doc_id not in DOCS:
                    print(f"INVALID DOC ID id: {doc_id} ON line: {line_number + 1} IN file: {filename}!")
                    continue

                indentation = len(line) - len(line.lstrip())

                if not line.endswith("\n"):
                    f.write("\n")
                f.write(into_comment(DOCS[doc_id], indentation))

def main() -> None:
    document_file("include/EasySockets/EasySockets.hpp")
    document_file("include/EasySockets/Windows/WindowsSocket.hpp")
    document_file("include/EasySockets/Windows/WinsockContext.hpp")
    document_file("include/EasySockets/Posix/PosixSocket.hpp")
    document_file("include/EasySockets/Posix/PosixContext.hpp")

if __name__ == "__main__":
    main()