# Realise - Git merge and conflict resolution

| | |
|---|---|
| **Author** | Matin Khajehfard |
| **Date** | April 2026 |
| **Version** | 1.0 |
| **Classification** | Internal |
| **Client** | City Sim Learning Group - HvA Smart Cities |
| **Company** | The Embedded Alliance |

---

## Introduction

This document records the merge of `main` into my `matin` branch. The branch was 233 commits behind. I had to resolve 5 file-level conflicts, work around Windows path limits, and clean up untracked file collisions. After the merge, I integrated Thijmen's railroad crossing code into the shared backend.

### Main question

How was the merge executed and what was integrated?

### Sub-questions

1. What conflicts existed and how were they resolved?
2. What new code was integrated from teammates?

---

## Chapter 1 - Merge conflicts and resolution

### Context

I ran `git merge origin/main` and immediately hit problems. Not just code conflicts. The OS blocked the merge before git could even start.

### Problems and fixes

**Windows long paths** - Git refused to check out files with paths longer than 260 characters. Teammate documentation folders had deep nesting. I ran `git config core.longpaths true` to fix it.

**Untracked file collisions** - I had local copies of teammate files that were now tracked on `main`. Git refused to overwrite them. I ran `git clean -fd` on the conflicting paths to remove the local copies.

**5 file-level conflicts:**

| File | My code | Thijmen's code | Resolution |
|------|---------|----------------|------------|
| `main.py` | Parking router import | Railroad router imports | Kept both imports |
| `models.py` | ParkingSpot model | Train + Barrier models | Kept all models |
| `schemas.py` | Parking schemas | Railroad schemas | Kept all schemas |
| `index.html` | Parking dashboard panel | Railroad dashboard panel | Kept both panels |
| `.gitignore` | My ignore patterns | Thijmen's ignore patterns | Merged both lists |

Every conflict followed the same pattern: my code and Thijmen's code needed to coexist. No conflict required choosing one side over the other.

**Stash conflicts** - I had stashed work before the merge. After the merge, `git stash pop` had its own conflicts because the stashed code assumed the pre-merge file structure. I resolved these manually.

### Sub-conclusion

The merge required 3 workarounds (long paths, clean, stash) and 5 file conflict resolutions. All resolved by keeping both sides.

---

## Chapter 2 - Integrated code

### New router groups

After the merge, the backend serves 4 router groups:

| Router | Prefix | Endpoints | Source |
|--------|--------|-----------|--------|
| Generic readings | `/api/v1/readings` | 3 | Sprint 1 (me) |
| Parking | `/api/v1/parking` | 4 | Sprint 1 (me) |
| Railroad train | `/api/v1/railroad/train` | 4 | Sprint 2 (Thijmen) |
| Railroad barrier | `/api/v1/railroad/barrier` | 4 | Sprint 2 (Thijmen) |

Total: 15+ REST endpoints across 4 router groups.

### New database models

**Train** - Stores train detection events from 3 IR sensors (A, B, C). Includes timestamps for each sensor trigger and a `predicted_arrival` field calculated from the time between sensors. Used for the animated approach visualization on the dashboard.

**Barrier** - Stores barrier state (up/down) with an `input_mode` field. A check constraint enforces valid values: `input_mode IN ('manual', 'train')`. This is a pattern I had not used before. It moves validation from Python into the database.

### Dashboard update

I added a railroad crossing panel to the dashboard. It shows real-time train approach with an animated progress line moving between sensors A, B, and C. When a train is detected at sensor A, the line starts moving toward C with a predicted arrival time displayed.

### Sub-conclusion

The backend now serves all team tiles. 2 new models with check constraints. Dashboard has 4 panels.

---

## Conclusion

The 233-commit merge is complete. 5 file conflicts resolved by keeping both sides. 4 router groups with 15+ endpoints. 2 new database models. The backend serves parking and railroad crossing data on the dashboard. The biggest lesson: do not let branches drift this far apart.

---

## References

- Git documentation: merge conflicts. https://git-scm.com/docs/git-merge
- SQLAlchemy check constraints. https://docs.sqlalchemy.org/
