-- Launch the setup workflow in the user's choice of terminal.
-- iTerm2 is listed first and preselected when both apps are installed.
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

    set availableTerminals to {}
    try
        set itermApplicationPath to path to application id "com.googlecode.iterm2"
        set end of availableTerminals to "iTerm2"
    end try
    try
        set terminalApplicationPath to path to application id "com.apple.Terminal"
        set end of availableTerminals to "Terminal"
    end try

    if (count of availableTerminals) = 0 then
        display alert "No supported terminal was found" message "Install iTerm2 or enable Terminal.app, then try again."
        return
    end if

    if (count of availableTerminals) = 1 then
        set selectedTerminal to item 1 of availableTerminals
    else
        set selectedTerminal to choose from list availableTerminals ¬
            with prompt "Choose where WiiCompiled Setup should show its build progress:" ¬
            default items {"iTerm2"} ¬
            OK button name "Open Setup" cancel button name "Cancel"
        if selectedTerminal is false then return
        set selectedTerminal to item 1 of selectedTerminal
    end if

    try
        if selectedTerminal is "iTerm2" then
            tell application id "com.googlecode.iterm2"
                activate
                create window with default profile command commandLine
            end tell
        else
            tell application id "com.apple.Terminal"
                activate
                do script commandLine
            end tell
        end if
    on error errorMessage number errorNumber
        display dialog "WiiCompiled Setup could not open " & selectedTerminal & "." & return & return & errorMessage & return & return & "Open WiiCompiled Setup again and choose the other terminal." buttons {"OK"} default button "OK" with icon caution
    end try
end run
