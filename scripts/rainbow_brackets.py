from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCES = [
    ("src/DataStructure/Splay.cpp", "build/rainbow/DataStructure/Splay.cpp"),
    ("src/Math/poly.cpp", "build/rainbow/Math/poly.cpp"),
    ("src/String/PAM.cpp", "build/rainbow/String/PAM.cpp"),
]

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
            out.append(char)
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
            out.append(char)
            if char == "\n":
                state = "code"
                line_can_start_directive = True
            i += 1
            continue

        if state == "block_comment":
            out.append(char)
            if char == "*" and nxt == "/":
                out.append(nxt)
                i += 2
                state = "code"
            else:
                i += 1
            continue

        if state in {"string", "char"}:
            out.append(char)
            if char == "\\" and nxt:
                out.append(nxt)
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
            out.append(char)

        if char == "\n":
            line_can_start_directive = True
        else:
            line_can_start_directive = False
        i += 1

    return "".join(out)


def main() -> None:
    for source, target in SOURCES:
        source_path = ROOT / source
        target_path = ROOT / target
        target_path.parent.mkdir(parents=True, exist_ok=True)
        target_path.write_text(
            rainbow_text(source_path.read_text(encoding="utf-8")),
            encoding="utf-8",
        )


if __name__ == "__main__":
    main()
