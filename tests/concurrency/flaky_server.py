"""Fake Maestro API that misbehaves the way EU did in SUP-8845.

Most requests succeed, but some are slow, some drop the connection without
answering (libcurl reports this as status -1, "Failed to query.") and some
send a truncated body. Uses only the standard library so it runs in the
builder image without pip.
"""

import random
import sys
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

OK_BODY = b'{"success":true}'


class FlakyHandler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def log_message(self, *args):
        pass

    def _handle(self):
        length = int(self.headers.get("Content-Length") or 0)
        if length:
            self.rfile.read(length)

        roll = random.random()
        if roll < 0.15:
            # Drop the connection without a response.
            self.close_connection = True
            return
        if roll < 0.25:
            # Promise a full body, send half of it, then hang up.
            self.send_response(200)
            self.send_header("Content-Type", "application/json")
            self.send_header("Content-Length", str(len(OK_BODY)))
            self.end_headers()
            self.wfile.write(OK_BODY[: len(OK_BODY) // 2])
            self.close_connection = True
            return
        if roll < 0.35:
            time.sleep(random.uniform(0.05, 0.2))

        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(OK_BODY)))
        self.end_headers()
        self.wfile.write(OK_BODY)

    do_GET = _handle
    do_PUT = _handle
    do_POST = _handle


if __name__ == "__main__":
    port = int(sys.argv[1]) if len(sys.argv) > 1 else 5000
    ThreadingHTTPServer(("127.0.0.1", port), FlakyHandler).serve_forever()
