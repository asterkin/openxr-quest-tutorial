import re
import sys
import yaml
from pathlib import Path

def scan_file(file_path):
    path = Path(file_path)
    if not path.exists():
        return {"error": "File not found"}

    content = path.read_text(encoding='utf-8')
    lines = content.splitlines()

    root = {
        "node": path.name,
        "path": str(path),
        "contains": [],
        "follows": [],
        "near": []
    }

    # Stack to manage hierarchy: [(level, dict_ref)]
    # We use a dummy root at level 0
    stack = [(0, root["contains"])]

    # Regex patterns
    header_pattern = re.compile(r'^(#+)\s+(.*)')
    link_pattern = re.compile(r'\[([^\]]+)\]\(([^)]+)\)')
    
    # Heuristics for relationship types
    # We look at the context (line before or current line text)
    
    for i, line in enumerate(lines):
        line = line.strip()
        
        # 1. Parse Headers (CONTAINS)
        header_match = header_match = header_pattern.match(line)
        if header_match:
            level = len(header_match.group(1))
            title = header_match.group(2).strip()
            
            new_node = {
                "name": title,
                "type": "section",
                "contains": [],
                "follows": [] # Section-level dependencies
            }
            
            # Pop stack until we find a parent with level < current level
            while stack and stack[-1][0] >= level:
                stack.pop()
            
            # Add to parent
            if stack:
                stack[-1][1].append(new_node)
            
            # Push new node
            stack.append((level, new_node["contains"]))
            continue

        # 2. Parse Links (FOLLOWS / NEAR)
        # We search for links in the line
        for match in link_pattern.finditer(line):
            text = match.group(1)
            target = match.group(2)
            
            # Context analysis
            # Check if line starts with specific keywords
            relation = "follows" # Default
            
            # Look at text immediately preceding the link or start of line
            pre_context = line[:match.start()].lower()
            
            if "see also" in pre_context or "references" in pre_context:
                relation = "near"
            elif "based on" in pre_context or "implements" in pre_context or "depends on" in pre_context:
                relation = "follows"
            
            # If target is HTTP, it's an external dependency (FOLLOWS)
            if target.startswith("http"):
                relation = "follows"
                type_ = "external"
            else:
                type_ = "internal"

            link_node = {
                "text": text,
                "target": target,
                "type": type_
            }
            
            # Add to the current active container (top of stack)
            # But we need to add to the 'follows' or 'near' list of the root or section?
            # The current stack item points to 'contains' list. 
            # We want to attach links to the *Section* that contains them.
            # Use the *parent* of the list at stack[-1]
            
            # Ideally we'd modify the structure to allow sections to have 'links'.
            # For simplicity, let's just add a 'link' entry to the 'contains' list for now,
            # or better: we can't easily access the parent dict from the list reference in python 
            # without changing the stack structure.
            
            # Let's just append a "link" node to the current 'contains' list
            stack[-1][1].append({
                "relation": relation,
                "target": target,
                "text": text,
                "context": pre_context.strip()
            })

    return root

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python sst_scanner.py <file_path>")
        sys.exit(1)
        
    result = scan_file(sys.argv[1])
    print(yaml.dump(result, sort_keys=False, default_flow_style=False))
