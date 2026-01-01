#!/bin/bash
# Claude Code Status Line Script
# Shows: git branch | context % free

# Use Python for JSON parsing (jq not available)
python -c "
import sys
import json

data = json.load(sys.stdin)

# Get git branch
cwd = data.get('workspace', {}).get('current_dir', '.')
import subprocess
try:
    result = subprocess.run(['git', 'branch', '--show-current'],
                          capture_output=True, text=True, cwd=cwd)
    branch = result.stdout.strip() or '(detached)'
except:
    branch = '(no git)'

# Calculate context free percentage
ctx = data.get('context_window', {})
usage = ctx.get('current_usage')
if usage:
    current = (usage.get('input_tokens', 0) +
               usage.get('cache_creation_input_tokens', 0) +
               usage.get('cache_read_input_tokens', 0))
    size = ctx.get('context_window_size', 200000)
    free_pct = 100 - (current * 100 // size)
else:
    free_pct = 100

print(f'{branch} | {free_pct}% free', end='')
"
