# Sprint 2 - Learning Goal 2: Merge conflicts and code integration

## Learning Question

How do I integrate team code from multiple branches when the merge has grown to 233 commits behind?

## S - Situation

My branch `matin` had fallen 233 commits behind `main`. Thijmen, Gurpreet, and Wesley had been pushing code while I worked on the backend in isolation. When I tried to merge, I got 5 file-level conflicts. On top of that, Windows threw long path errors and untracked files from teammates blocked the merge.

This was the first time the team had to integrate code from all branches into one working backend.

## T - Task

Merge `main` into `matin`. Resolve all conflicts. Integrate Thijmen's railroad crossing API into the shared backend. Update the dashboard to show the new tile.

### Deliverables

**Realise document** - [Realise - Git merge and conflict resolution](Realise%20-%20Git%20merge%20and%20conflict%20resolution.md)

The merge execution, conflict resolution, and integrated code.

## A - Action

I pulled `main` and immediately hit `Filename too long` errors on Windows. Enabled `core.longpaths` in git config. Then untracked files from teammates clashed with tracked files on main. I ran `git clean -fd` on the conflicting paths.

The actual merge had 5 file conflicts: `main.py`, `models.py`, `schemas.py`, `index.html`, and `.gitignore`. For each one, I kept both sides. My parking code and Thijmen's railroad code needed to coexist, not replace each other.

After the merge, I had stash conflicts from code I had temporarily stored. Resolved those manually too.

Then I verified the integrated code: 4 router groups (generic readings, parking, railroad train, railroad barrier), 15+ endpoints, 2 new database models (`Train` and `Barrier` with check constraints), and a new dashboard panel for the railroad crossing with animated train approach visualization.

Documented everything in the [Realise](Realise%20-%20Git%20merge%20and%20conflict%20resolution.md).

## R - Result

The [Realise](Realise%20-%20Git%20merge%20and%20conflict%20resolution.md) shows the merge completed successfully. The backend now serves all tiles. The dashboard has a railroad crossing panel with real-time train detection, animated progress line between sensors A/B/C, and predicted arrival time.

## R - Reflection

233 commits behind is too many. The merge took hours instead of minutes because I had been working in isolation for too long. I should have merged `main` into my branch weekly.

The Windows long path issue was frustrating. I did not expect OS-level problems blocking a git merge. Now I know to set `core.longpaths` on Windows from the start.

I also learned that merge conflicts are not scary if you understand both sides. In every conflict, I could see my code and Thijmen's code clearly. The resolution was always "keep both."

## T - Transfer

For Sprint 3, I will merge `main` into my branch at least once per week. No more letting it drift to 233 commits. I will also set `core.longpaths` in the global git config so it never blocks me again.

## References

Matin. (2026). Realise: Git merge and conflict resolution [Realise deliverable]. [Realise - Git merge and conflict resolution](Realise%20-%20Git%20merge%20and%20conflict%20resolution.md)
