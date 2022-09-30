The folders and files under config directory shall be placed IAR workbench directories. for example:

All the files in folder lib\devices shall be placed to:
C:\Program Files\IAR Systems\Embedded Workbench 8.1\arm\config\devices\IndieSemi\

All the files in folder lib\flashloader shall be placed to:
C:\Program Files\IAR Systems\Embedded Workbench 8.1\arm\config\flashloader\IndieSemi\


When you run any demo program first time, it might see some warnings from IAR IDE about broken options in the project files.
It mostly means that your IAR workbench version is different from the version used to create the demo code.
You can simply ignore the warnings, the workbench will create a backup for the old project files and create new ones to match your workbench version.