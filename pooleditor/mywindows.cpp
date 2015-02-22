LRESULT CALLBACK DlgOpening(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
ShowCursor(TRUE);
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_COMMAND:									// If we clicked on anything in the dialog box

			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:
					{
					if (IDC_FULLSCREENCHECK == true)
						fullscreen = true;
					else
						fullscreen = false;
					EndDialog( hWnd, FALSE );
					break;
					}
				case IDCANCEL:
					done = true;
					EndDialog( hWnd, FALSE );
			}
	}
	return false;
ShowCursor(FALSE);
}

LRESULT CALLBACK DlgOK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
ShowCursor(TRUE);
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_COMMAND:									// If we clicked on anything in the dialog box

			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:									// Check if the OK button was clicked																			                    
				EndDialog( hWnd, FALSE );
			}
	}
	return false;
}

LRESULT CALLBACK DlgPasswordProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	ShowCursor(TRUE);
	char szPassword[25]={0};
	char tempfile[25]={0};
    switch( message )										// Switch on all of the dialogs messages
    {
		case WM_INITDIALOG:									// If Initilizing the dialog box		

			// Do initialization here	(like WM_CREATE)

            return TRUE;

		case WM_COMMAND:									// If we clicked on anything in the dialog box
            
			switch( LOWORD( wParam ) )						// Check the LOWORD of the wParam (Which holds the ID of what was clicked on)
            {    
				case IDOK:									// Check if the OK button was clicked																			                    
															// This gets what the user typed into the password field.
															// It takes the hWnd, the ID of the dialog box control, a string to hold what they typed in,
															// and how many characters you want to retrieve from the field.
					GetDlgItemText(hWnd, IDC_PASSWORD, szPassword, 25);
															
															// Check if they typed in the right password
					if(strcmp(szPassword, "\0"))
					{	
							strcpy (tempfile,"Maps/");
				strncat (tempfile, szPassword, strlen(szPassword));
				strncat (tempfile, ".dlf", 4);

				*templinex = 0.0f; *templiney = 0.0f;
				int count;
				for (int i = 0; i < MAX_WALLS; i++)
				{
					if (wall[i].linex != 0)
						count = i+1;
				}
				ofstream fout(tempfile);					
				fout << count << "\n";
				fout << ballcount << "\n";
				fout << holecount << "\n";
				fout.close();

				//save geometry
				for (i = 0; i < count; i++)
				{
					wall[i].WriteFile(tempfile);
				}
				for (i = 0; i < ballcount; i++)
				{
					ball[i].WriteFile(tempfile);
				}
				for (i = 0; i < holecount; i++)
				{
					hole[i].WriteFile(tempfile);
				}

				output.tempx = 0.0f;
				saving = false;

						EndDialog( hWnd, FALSE );			// Close the dialog box	
					}
					return TRUE;							// Return from the dialog proc

				case IDCANCEL:								// Check if the cancel button was pressed
					EndDialog( hWnd, FALSE );										// Display a message box saying we clicked cancel.  (MB_OK stands for message box with a OK button)					EndDialog( hWnd, FALSE );				// Close the dialog box

					return TRUE;							// Quit from this function
            }
            break;

        case WM_CLOSE:										// If we close the dialog box
			//password = false;
			EndDialog( hWnd, FALSE );						// Close the dialog box
            
			break;

		case WM_DESTROY:									// This message happens when the dialog box closes          
        
			// If we need to free anything, do it here

			break;										// Break from the loop
    }
	ShowCursor(FALSE);
    return FALSE;											// Return a default false
}