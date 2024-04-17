from contextlib import contextmanager
import ctypes
import io
import os
import sys
import tempfile

libc = ctypes.CDLL(None)
c_stdout = ctypes.c_void_p.in_dll(libc, 'stdout')
c_stderr = ctypes.c_void_p.in_dll(libc, 'stderr')

@contextmanager
def Redirector(out_stream, err_stream):
    original_stdout_fd = sys.stdout.fileno()
    original_stderr_fd = sys.stderr.fileno()

    def _redirect_stdout(to_fd):
        """Redirect stdout to the given file descriptor."""
        # Flush the C-level buffer stdout
        libc.fflush(c_stdout)
        # Flush and close sys.stdout - also closes the file descriptor (fd)
        sys.stdout.close()
        # Make original_stdout_fd point to the same file as to_fd
        os.dup2(to_fd, original_stdout_fd)
        # Create a new sys.stdout that points to the redirected fd
        sys.stdout = io.TextIOWrapper(os.fdopen(original_stdout_fd, 'wb'))

    def _redirect_stderr(to_fd):
        """Redirect stdout to the given file descriptor."""
        # Flush the C-level buffer stdout
        libc.fflush(c_stderr)
        # Flush and close sys.stdout - also closes the file descriptor (fd)
        sys.stderr.close()
        # Make original_stderr_fd point to the same file as to_fd
        os.dup2(to_fd, original_stderr_fd)
        # Create a new sys.stderr that points to the redirected fd
        sys.stderr = io.TextIOWrapper(os.fdopen(original_stderr_fd, 'wb'))

    saved_stdout_fd = None
    saved_stderr_fd = None
    tmp_stdout_file = None
    tmp_stderr_file = None
    try:
        if out_stream:
            # Create a temporary file and redirect stdout to it
            saved_stdout_fd = os.dup(original_stdout_fd)
            tmp_stdout_file = tempfile.TemporaryFile(mode='w+b')
            _redirect_stdout(tmp_stdout_file.fileno())
        if err_stream:
            # Create a temporary file and redirect stderr to it
            saved_stderr_fd = os.dup(original_stderr_fd)
            tmp_stderr_file = tempfile.TemporaryFile(mode='w+b')
            _redirect_stderr(tmp_stderr_file.fileno())

        yield

    finally:
        if out_stream:
            _redirect_stdout(saved_stdout_fd)
            # Copy contents of temporary file to the given stream
            tmp_stdout_file.flush()
            tmp_stdout_file.seek(0, io.SEEK_SET)
            out_stream.write(tmp_stdout_file.read().decode("utf-8") )
        if err_stream:
            _redirect_stderr(saved_stderr_fd)
            # Copy contents of temporary file to the given stream
            tmp_stderr_file.flush()
            tmp_stderr_file.seek(0, io.SEEK_SET)
            err_stream.write(tmp_stderr_file.read().decode("utf-8") )

        if tmp_stdout_file is not None:
            tmp_stdout_file.close()
        if saved_stdout_fd is not None:
            os.close(saved_stdout_fd)
        if tmp_stderr_file is not None:
            tmp_stderr_file.close()
        if saved_stderr_fd is not None:
            os.close(saved_stderr_fd)
