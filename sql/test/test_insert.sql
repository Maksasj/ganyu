-- Checking constraints
    -- Typo in source type
    INSERT INTO maja8801.Source(sourceName, sourceDescription, sourceType, sourceRootDestination)
    VALUES
        ('GoogleDrive', 'my old google drive', 'WEBB', 'https://drive.google.com/drive/my-drive');
    -- File references not existing source
    INSERT INTO maja8801.VirtualFile(FileName, FileExtension, FileSize, ContentHash, DateCreated, DateModified, SourceID, SourceAccess)
        VALUES
        ('test', '.txt', 1, 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa', DATE '2024-05-15', DATE '2024-05-15', 99, 'ops');
    -- File name AND file extensions are empty
    INSERT INTO maja8801.VirtualFile(FileName, FileExtension, FileSize, ContentHash, DateCreated, DateModified, SourceID, SourceAccess)
        VALUES
        (NULL, NULL, 1, 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa', DATE '2024-05-15', DATE '2024-05-15', 1, 'ops0');
    INSERT INTO maja8801.VirtualFile(FileName, FileExtension, FileSize, ContentHash, DateCreated, DateModified, SourceID, SourceAccess)
        VALUES
        (NULL, '', 1, 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa', DATE '2024-05-15', DATE '2024-05-15', 1, 'ops1');
    INSERT INTO maja8801.VirtualFile(FileName, FileExtension, FileSize, ContentHash, DateCreated, DateModified, SourceID, SourceAccess)
        VALUES
        ('', NULL, 1, 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa', DATE '2024-05-15', DATE '2024-05-15', 1, 'ops2');
    INSERT INTO maja8801.VirtualFile(FileName, FileExtension, FileSize, ContentHash, DateCreated, DateModified, SourceID, SourceAccess)
        VALUES
        ('', '', 1, 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa', DATE '2024-05-15', DATE '2024-05-15', 1, 'ops3');
    -- Storing not existing file in existing directory
    INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
        VALUES
        (999999, 1);
    -- Storing file in not existing directory
    INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
        VALUES
        (1, 999999);
    -- Storing not existing file in not existing directory
    INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
        VALUES
        (999999, 999999);
    -- Storing not existing directory in existing directory
    INSERT INTO maja8801.VirtualDirectoryStored(VirtualDirectoryIDConstraint, TargetVirtualDirectoryIDConstraint)
        VALUES
        (999999, 1);
    -- Storing directory in not existing directory
    INSERT INTO maja8801.VirtualDirectoryStored(VirtualDirectoryIDConstraint, TargetVirtualDirectoryIDConstraint)
        VALUES
        (1, 999999);
    -- Storing not existing directory in not existing directory
    INSERT INTO maja8801.VirtualDirectoryStored(VirtualDirectoryIDConstraint, TargetVirtualDirectoryIDConstraint)
        VALUES
        (999999, 999999);
    
-- Checking triggers
    -- Trying to store Documents inside Office directory, meanwhile Office is already stored in Documents 
    INSERT INTO maja8801.VirtualDirectoryStored(VirtualDirectoryID, TargetVirtualDirectoryID)
    VALUES
        (6, 7);

    -- Same to first test, but depth is 2 directories
    INSERT INTO maja8801.VirtualDirectoryStored(VirtualDirectoryID, TargetVirtualDirectoryID)
    VALUES
        (6, 8);