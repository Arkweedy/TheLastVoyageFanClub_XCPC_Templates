from pathlib import Path
import shutil


ROOT = Path(__file__).resolve().parents[1]
SOURCE_ROOT = ROOT / "src"
TARGET_ROOT = ROOT / "build" / "rainbow"

OPEN = "([{"
CLOSE = ")]}"
DISPLAY = {
    "(": "(",
    ")": ")",
    "[": "[",
    "]": "]",
    "{": r"\{",
    "}": r"\}",
}


def rainbow_token(char: str, color: int) -> str:
    return f"@\\TemplateRainbowBracket{{{color}}}{{{DISPLAY[char]}}}@"


def escaped_at() -> str:
    return r"@\char64{}@"


def rainbow_text(text: str) -> str:
    stack: list[int] = []
    out: list[str] = []
    state = "code"
    line_can_start_directive = True
    preprocessor_prev = ""
    i = 0

    while i < len(text):
        char = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""

        if state == "preprocessor":
            out.append(escaped_at() if char == "@" else char)
            if char == "\n":
                if preprocessor_prev != "\\":
                    state = "code"
                line_can_start_directive = True
                preprocessor_prev = ""
            else:
                if char != "\r":
                    preprocessor_prev = char
            i += 1
            continue

        if state == "line_comment":
            out.append(escaped_at() if char == "@" else char)
            if char == "\n":
                state = "code"
                line_can_start_directive = True
            i += 1
            continue

        if state == "block_comment":
            out.append(escaped_at() if char == "@" else char)
            if char == "*" and nxt == "/":
                out.append(nxt)
                i += 2
                state = "code"
            else:
                i += 1
            continue

        if state in {"string", "char"}:
            out.append(escaped_at() if char == "@" else char)
            if char == "\\" and nxt:
                out.append(escaped_at() if nxt == "@" else nxt)
                i += 2
                continue
            if (state == "string" and char == '"') or (state == "char" and char == "'"):
                state = "code"
            i += 1
            continue

        if line_can_start_directive and char in " \t\r":
            out.append(char)
            i += 1
            continue

        if line_can_start_directive and char == "#":
            out.append(char)
            i += 1
            state = "preprocessor"
            line_can_start_directive = False
            preprocessor_prev = "#"
            continue

        if char == "/" and nxt == "/":
            out.extend((char, nxt))
            i += 2
            state = "line_comment"
            continue

        if char == "/" and nxt == "*":
            out.extend((char, nxt))
            i += 2
            state = "block_comment"
            continue

        if char == '"':
            out.append(char)
            i += 1
            state = "string"
            line_can_start_directive = False
            continue

        if char == "'":
            out.append(char)
            i += 1
            state = "char"
            line_can_start_directive = False
            continue

        if char in OPEN:
            color = len(stack) % 4
            stack.append(color)
            out.append(rainbow_token(char, color))
        elif char in CLOSE:
            color = stack.pop() if stack else 0
            out.append(rainbow_token(char, color))
        else:
            out.append(escaped_at() if char == "@" else char)

        if char == "\n":
            line_can_start_directive = True
        else:
            line_can_start_directive = False
        i += 1

    return "".join(out)


def main() -> None:
    if TARGET_ROOT.exists():
        shutil.rmtree(TARGET_ROOT)

    sources = sorted(SOURCE_ROOT.rglob("*.cpp"))
    for source_path in sources:
        target_path = TARGET_ROOT / source_path.relative_to(ROOT)
        target_path.parent.mkdir(parents=True, exist_ok=True)
        target_path.write_text(
            rainbow_text(source_path.read_text(encoding="utf-8")),
            encoding="utf-8",
        )
    print(f"Generated rainbow brackets for {len(sources)} C++ files.")


if __name__ == "__main__":
    main()
