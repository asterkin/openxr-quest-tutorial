#!/bin/bash
# Claude Code Status Line Script
# Shows: git branch | ctx:XX% ses:XX% (both show remaining/free)

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

# Calculate context free percentage
ctx = data.get('context_window', {})
usage = ctx.get('current_usage')
if usage:
    current = (usage.get('input_tokens', 0) +
               usage.get('cache_creation_input_tokens', 0) +
               usage.get('cache_read_input_tokens', 0))
    size = ctx.get('context_window_size', 200000)
    ctx_pct = 100 - (current * 100 // size)
else:
    ctx_pct = 100

# Calculate session time remaining percentage (of 5h = 18,000,000ms)
cost = data.get('cost', {})
duration_ms = cost.get('total_duration_ms', 0)
session_limit_ms = 5 * 60 * 60 * 1000  # 5 hours
ses_pct = max(0, 100 - (duration_ms * 100 // session_limit_ms)) if session_limit_ms else 100

print(f'{branch} | ctx:{ctx_pct}% ses:{ses_pct}%', end='')
"
