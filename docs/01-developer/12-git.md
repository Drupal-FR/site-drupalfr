# Git configuration

## Global settings

### Mandatory

```bash
git config --global user.name "name"
git config --global user.email "mail@mail.mail"
git config --global core.editor "vim"
git config --global color.ui true
git config --global core.autocrlf false
git config --global core.safecrlf false
git config --global core.ignorecase false
git config --global diff.renames copies
git config --global alias.st 'status'
git config --global alias.co 'checkout'
git config --global alias.up 'pull --rebase'
git config --global alias.sl 'log --graph --pretty=oneline --abbrev-commit --decorate'
git config --global alias.slp 'log --graph --pretty=format:"%h%x09%an%x09%ad%x09%s" --abbrev-commit --decorate'
git config --global alias.lg "log --graph --date=relative --pretty=tformat:'%Cred%h%Creset -%C(auto)%d%Creset %s %Cgreen(%an %ad)%Creset'"
git config --global alias.lga "log --graph --abbrev-commit --pretty=format:'%C(red)%h%Creset -%C(yellow)%d%Creset %s %C(green)(%cr) %C(bold blue)<%an>%Creset' --all"
```

Note: you can use a different editor than vim.

### Optional

If you want to ignore file permissions changes:
```bash
git config --global core.filemode false
```

To apply the `--rebase` option automatically:
```bash
git config --global branch.autosetuprebase always
```

## Global gitignore

Create a global `.gitignore` file where you want, the name is arbitrary.

It will be used for all git repositories, put the following content inside:
```
# Patch/diff artifacts.
*.patch
*.diff
*.orig
*.rej
interdiff*.txt

# emacs artifacts.
*~
\#*\#

# VI swap file.
*.swp

# Hidden files.
.DS*
.project

# Windows links.
*.lnk

# Temporary files.
tmp*

# Exclude IDE's project directory.
nbproject
.idea

# Package management systems.
bower_components
node_modules
vendor
yarn-error.log
```

Adapt the following command to the path and file name of your previously created
file:
```bash
git config --global core.excludesfile ~/.gitignore
```
