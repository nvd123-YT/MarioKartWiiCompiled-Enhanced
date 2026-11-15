-- Launch the setup workflow in Terminal so the user can see build progress.
on run argv
    if (count of argv) < 1 then
        display alert "WiiCompiled Setup could not start" message "The setup script path was not supplied."
        return
    end if

    set commandLine to quoted form of (item 1 of argv)
    if (count of argv) > 1 then
        repeat with argumentIndex from 2 to (count of argv)
            set commandLine to commandLine & " " & quoted form of (item argumentIndex of argv)
        end repeat
    end if

    tell application id "com.apple.Terminal"
        activate
        do script commandLine
    end tell
end run
