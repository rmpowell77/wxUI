---

Checklist

Did all the examples build and run?

Did you run the commands `checklists/check_copyright` and `checklists/check_md`?

Did you make sure the LICENSE.md is the correct date?

Did you run:

```
docs/src/make_md.py docs/src/README.md > README.md
docs/src/make_md.py docs/src/docs/ProgrammersGuide.md > docs/ProgrammersGuide.md 
```

---

# How to release wxUI:

wxUI uses CMake and Github actions to automate the release process.  When you push a tag for the repository, github will build and package the release.

The current version can by found by running `git describe --tags --abbrev=0`.  You should pick an appropriate incremented version, trying to follow the guidance of [https://semver.org](https://semver.org).  In all commands below, substitute that number for `$VERS` (meaning when you read `$VERS`).

 1. Prebuild the project to make sure its working

 2. Update LATEST_RELEASE_NOTES.md with all the changes (it will get posted to the release by our CI bot)

 3. Tag the depot

```
$ git tag -a $VERS -m "wxUI-$VERS"
$ git push origin $VERS
```

This should trigger the github action, which should publish release notes in Draft form.

 4. Once the Release information looks good, Press the Publish Release button on github.

 5. Update https://github.com/rmpowell77/wxUI/milestone by closing the shipped milestone and starting the next one.

 6. Notify the world about the new version of wxUI.

 7. Have a cookie.

