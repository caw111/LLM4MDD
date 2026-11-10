#!/usr/bin/env python3
import argparse
import json
import sys
import urllib.error
import urllib.request


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate subsystem_ir.json via FastAPI.")
    parser.add_argument("--prompt-file", required=True, help="Path to prompt file (UTF-8).")
    parser.add_argument("--url", default="http://localhost:8000/generate/subsystem-ir", help="Endpoint URL.")
    parser.add_argument("--model", default=None, help="Optional model override.")
    parser.add_argument("--out-ir", default="outputs/subsystem_ir.json", help="Output path for subsystem_ir.json.")
    parser.add_argument("--timeout", type=int, default=120, help="Request timeout in seconds.")
    parser.add_argument("--quiet", action="store_true", help="Do not print response JSON.")
    args = parser.parse_args()

    payload = {"prompt_file": args.prompt_file, "out_ir_path": args.out_ir}
    if args.model:
        payload["model"] = args.model

    data = json.dumps(payload).encode("utf-8")
    request = urllib.request.Request(
        args.url,
        data=data,
        headers={"Content-Type": "application/json"},
        method="POST",
    )

    try:
        with urllib.request.urlopen(request, timeout=args.timeout) as response:
            body = response.read().decode("utf-8")
    except urllib.error.HTTPError as exc:
        sys.stderr.write(f"HTTP {exc.code}: {exc.read().decode('utf-8', errors='ignore')}\n")
        return 1
    except urllib.error.URLError as exc:
        sys.stderr.write(f"Request failed: {exc}\n")
        return 1

    if not args.quiet:
        print(body)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

