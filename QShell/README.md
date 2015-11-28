# QShell

A shell-like plug and play widget made with Qt, supporting QtScript by default. My requirement was to have a reusable widget ready to 
be promoted from QPlainTextEdit.

Some features:

* JS-like syntax highlighting 

* QtScript integration by default:
   * evaluation
   * print function
   * load script function
   * exit function

[![https://gyazo.com/e3e7caf0483b197eb40d760348e5184c](https://i.gyazo.com/e3e7caf0483b197eb40d760348e5184c.gif)](https://gyazo.com/e3e7caf0483b197eb40d760348e5184c)

* Naive intellisense (by default is by JS reflection)

[![https://gyazo.com/82ab80a90e473b4ebe6a6536acb8e4c3](https://i.gyazo.com/82ab80a90e473b4ebe6a6536acb8e4c3.gif)](https://gyazo.com/82ab80a90e473b4ebe6a6536acb8e4c3)

* traditional shell behavior:
   * history recording
   * customizable prompt string
   * multi-line edit
   * Matlab-like clc command
   * zoom-in/zoom-out
   * drag-and-drop

# Disclaimer

I coded this project in a couple of weeks during 2011. It does the job. 
Now I realize it's over designed and it could be done with much less effort and a few classes, 
but I don't have time to redo this component.

At that time I also wrote both unit tests and end-to-end tests (e.g. GUI interaction) by using the QTestLib module. 
I'll make them available on a repo fully dedicated to QShell.
