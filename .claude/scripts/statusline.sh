#!/bin/bash
# Claude Code Status Line Script
# Shows: git branch | ctx:XX% (context utilization)

# Use Python for JSON parsing (jq not available)
python -c "
import sys
import json
import subprocess

data = json.load(sys.stdin)

# Get git branch
cwd = data.get('workspace', {}).get('current_dir', '.')
try:
    result = subprocess.run(['git', 'branch', '--show-current'],
                          capture_output=True, text=True, cwd=cwd)
    branch = result.stdout.strip() or '(detached)'
except:
    branch = '(no git)'

# Calculate context FREE percentage (remaining, matches /context output)
ctx = data.get('context_window', {})
size = ctx.get('context_window_size', 200000)

# Check if current_usage is available (null after /clear or restart)
usage = ctx.get('current_usage')
if usage is None:
    print(f'{branch} | free:...', end='')
else:
    current = (usage.get('input_tokens', 0) +
               usage.get('cache_creation_input_tokens', 0) +
               usage.get('cache_read_input_tokens', 0))
    free_pct = max(0.0, 100 - (current * 100 / size)) if size else 100
    print(f'{branch} | free:{free_pct:.1f}%', end='')
"
