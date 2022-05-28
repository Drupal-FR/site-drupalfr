# Development workflow

## Development

**Important note:** This workflow assume that you are using Core's configuration
management system. If this is note the case, adapt the commands/steps to your
workflow.

When working on a new issue:
1. Create a new local branch: `git checkout -b pseudo-issue_number-short_description --track origin/develop`
2. Update your local environment to ensure to have up-to-date vendor sources and
   imported config: `make docker-site-update SELECTED_SITE=all`
3. Do your development
4. If the development implies configuration changes: `make docker-shell-web-cmd "drush @default.alias cex -y"`
5. Ensure it passes code quality: `make quality-all`
6. Commit your changes
    * The commit message should follow the pattern: `Issue #ticket_number: Description.`
7. Rebase from develop: `git pull --rebase origin/develop`
8. Update locally to ensure your development is still ok: `make docker-site-update SELECTED_SITE=all`
9. Re-test locally
10. When ok, push your changes: `git push origin pseudo-issue_number-short_description`
11. Go on Gitlab and create a merge request

**Warning:** when switching branch to start a new ticket, if your previous ticket
introduced new contrib or custom modules, uninstall it before switching branches
because the code will not be present on the new local branch.

Note: the commands should be adapted if the target branch is not "develop" and
if the website on which development happens is not or not only the "default"
website.

## Code review

### MR author

* When creating a merge request:
  * put a link to the bug tracker issue in the MR description
  * 99% of the time MR will be against the "develop" branch.
* After saving the MR:
  * update the bug tracker issue to put a comment with a link to the MR
  * ensure the MR is rebased
  * assign the MR to the reviewer when the CI is ok
  * update the bug tracker issue status and assign it to the reviewer
* Your MR had been rejected:
  * place yourself back on the Git branch, and fix the points
  * **do not mark the review points as resolved**, it is the reviewer's responsibility.
    You can add a comment into the points to indicate that you have taken it
    into account and for yourself to not forget points to fix

### Reviewer

After reviewing a merge request:
* if it is back from a previous code review with blocking points:
  * mark the resolved points as resolved.
* no blocking point:
  * you can merge it, and squash the commits if needed/desired
  * when merged you can unassigned the MR and update the bug tracker issue.
* one or more blocking points:
  * re-assign the MR to the author
  * update the bug tracker issue status and re-assign it to the MR author.
