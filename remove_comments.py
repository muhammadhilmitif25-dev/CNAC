from pathlib import Path

p = Path('.')
files = [*p.glob('*.c'), *p.glob('*.h')]


def remove_comments(code: str) -> str:
    out = []
    i = 0
    n = len(code)
    while i < n:
        c = code[i]
        if c == '"' or c == "'":
            quote = c
            out.append(c)
            i += 1
            while i < n:
                out.append(code[i])
                if code[i] == '\\':
                    i += 1
                    if i < n:
                        out.append(code[i])
                    i += 1
                    continue
                if code[i] == quote:
                    i += 1
                    break
                i += 1
        elif c == '/' and i + 1 < n:
            nxt = code[i + 1]
            if nxt == '/':
                i += 2
                while i < n and code[i] != '\n':
                    i += 1
            elif nxt == '*':
                i += 2
                while i + 1 < n and not (code[i] == '*' and code[i + 1] == '/'):
                    i += 1
                i += 2
            else:
                out.append(c)
                i += 1
        else:
            out.append(c)
            i += 1
    return ''.join(out)

for file in files:
    text = file.read_text(encoding='utf-8')
    new = remove_comments(text)
    if new != text:
        file.write_text(new, encoding='utf-8')
        print(f'cleaned {file}')
