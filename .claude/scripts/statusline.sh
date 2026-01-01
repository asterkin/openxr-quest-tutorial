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

# Calculate context usage percentage (USED, not remaining)
ctx = data.get('context_window', {})
size = ctx.get('context_window_size', 200000)

# Try current_usage first, then fall back to total tokens
usage = ctx.get('current_usage')
if usage:
    current = (usage.get('input_tokens', 0) +
               usage.get('cache_creation_input_tokens', 0) +
               usage.get('cache_read_input_tokens', 0))
else:
    # Fallback: use total_input_tokens as approximation
    current = ctx.get('total_input_tokens', 0)

ctx_pct = min(100, current * 100 // size) if size else 0

print(f'{branch} | ctx:{ctx_pct}%', end='')
"
