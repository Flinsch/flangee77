import json
import re
import shutil
import subprocess
import sys


def run_doxygen():
    result = subprocess.call(['doxygen', 'Doxyfile'])

    if result != 0:
        print('Doxygen encountered an error.')
        sys.exit(result)

    print('Doxygen ran successfully.')


def inject_script_tag(html_path, script_path):
    with open(html_path, 'r', encoding='utf-8') as file:
        content = file.read()

    # Inject the script tag before the closing </head> tag.
    new_content = re.sub(
        r'(</head>)',
        f'<script type="text/javascript" src="{script_path}"></script>\n\\1',
        content
    )

    with open(html_path, 'w', encoding='utf-8') as file:
        file.write(new_content)

    print(f"Injected script tag '{script_path}' into '{html_path}'.")


def inject_custom_script(js_path, script_path):
    with open(script_path, 'r') as fsrc:
        with open(js_path, 'a') as fdst:
            shutil.copyfileobj(fsrc, fdst)

    print(f"Injected '{script_path}' into '{js_path}'.")


def modify_navtreedata(js_path):
    with open(js_path, 'r', encoding='utf-8') as file:
        content = file.read()

    # Extract JSON-like structure using regex.
    match = re.search(
        r'var NAVTREE =\s*(\[.*?\]);',
        content,
        flags=re.DOTALL
    )

    if not match:
        print(f"Could not find `NAVTREE` in file '{js_path}'.")
        return

    nav_data = json.loads(match.group(1))

    # Rename main page.
    nav_data[0][2][0][0] = 'Overview'

    # Reconstruct and write back.
    new_content = re.sub(
        r'var NAVTREE =\s*(\[.*?\]);',
        f'var NAVTREE = {json.dumps(nav_data, indent=2)};',
        content,
        flags=re.DOTALL
    )

    with open(js_path, 'w', encoding='utf-8') as file:
        file.write(new_content)



    print(f"Modified '{js_path}' successfully.")


if __name__ == '__main__':
    run_doxygen()
    #inject_script_tag('html/index.html', 'custom.js')
    inject_custom_script('html/navtree.js', 'custom-navtree.js')
    modify_navtreedata('html/navtreedata.js')

    print('Doxygen customization complete!')
