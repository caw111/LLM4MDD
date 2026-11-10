#!/usr/bin/env python3
import argparse
import json
import sys
import urllib.error
import urllib.request


def main() -> int:
    parser = argparse.ArgumentParser(description="Send a prompt to the FastAPI /chat endpoint.")
    parser.add_argument("--prompt-file", default="prompt.txt", help="Path to prompt text file.")
    parser.add_argument("--url", default="http://localhost:8000/chat", help="Chat endpoint URL.")
    parser.add_argument("--model", default=None, help="Optional model override.")
    parser.add_argument("--out", default=None, help="Optional output file for the text response.")
    parser.add_argument("--raw", action="store_true", help="Print raw JSON instead of text only.")
    parser.add_argument("--quiet", action="store_true", help="Do not print response text to stdout.")
    parser.add_argument("--timeout", type=int, default=60, help="Request timeout in seconds.")
    args = parser.parse_args()

    try:
        with open(args.prompt_file, "r", encoding="utf-8") as handle:
            prompt = handle.read()
    except OSError as exc:
        sys.stderr.write(f"Failed to read prompt file: {exc}\n")
        return 1

    payload = {"prompt": prompt}
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

    if args.raw:
        print(body)
        return 0

    try:
        obj = json.loads(body)
        text = obj.get("text", "")
    except json.JSONDecodeError:
        text = body

    if not args.quiet:
        print(text)
    if args.out:
        try:
            with open(args.out, "w", encoding="utf-8") as handle:
                handle.write(text)
        except OSError as exc:
            sys.stderr.write(f"Failed to write output file: {exc}\n")
            return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
