// ���ɾ�			�ɼ�				������ �ִ� ���			���纻�� ������ ���

xcopy			/y				.\Engine\Header\*.h			.\Reference\Header\
xcopy			/y				.\Engine\Header\*.inl		.\Reference\Header\


xcopy			/y				.\Engine\System\Code\*.h	.\Reference\Header\
xcopy			/y				.\Engine\Utility\Code\*.h	.\Reference\Header\

//Client
xcopy			/y				.\Engine\System\Bin\System.dll		.\Client\Bin\
xcopy			/y				.\Engine\Utility\Bin\Utility.dll	.\Client\Bin\

//Tool
xcopy			/y				.\Engine\System\Bin\System.dll		.\Tool\Bin\
xcopy			/y				.\Engine\Utility\Bin\Utility.dll	.\Tool\Bin\

//Reference
xcopy			/y				.\Engine\System\Bin\System.lib		.\Reference\Library\
xcopy			/y				.\Engine\Utility\Bin\Utility.lib	.\Reference\Library\