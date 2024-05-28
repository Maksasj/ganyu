-- Checking constraints
    -- Typo in source type
        UPDATE maja8801.Source
            SET sourceType = 'WEBB'
        WHERE sourceType = 'WEB';
    -- File references not existing source
        UPDATE maja8801.VirtualFile
            SET SourceID = 999999
        WHERE SourceID = 1;
    -- File name AND file extensions are empty
        UPDATE maja8801.VirtualFile
            SET FileName = NULL, FileExtension = NULL
        WHERE SourceID = 1;
        UPDATE maja8801.VirtualFile
            SET FileName = '', FileExtension = NULL
        WHERE SourceID = 1;
        UPDATE maja8801.VirtualFile
            SET FileName = NULL, FileExtension = ''
        WHERE SourceID = 1;
        UPDATE maja8801.VirtualFile
            SET FileName = '', FileExtension = ''
        WHERE SourceID = 1;
    -- Storing not existing file in existing directory
        UPDATE maja8801.VirtualFileStored
            SET VirtualFileID = 999999
        WHERE VirtualFileID = 1192; -- File exist
    -- Storing file in not existing directory
        UPDATE maja8801.VirtualFileStored
            SET VirtualDirectoryID = 999999
        WHERE VirtualFileID = 1192; -- File exist
    -- Storing not existing file in not existing directory
        UPDATE maja8801.VirtualFileStored
            SET VirtualFileID = 999999, VirtualDirectoryID = 999999
        WHERE VirtualFileID = 1192; -- File exist
    -- Storing not existing directory in existing directory
        UPDATE maja8801.VirtualDirectoryStored
            SET VirtualDirectoryID = 999998
        WHERE VirtualDirectoryID = 1; -- File exist
    -- Storing directory in not existing directory
        UPDATE maja8801.VirtualDirectoryStored
            SET TargetVirtualDirectoryID = 999998
        WHERE VirtualDirectoryID = 1; -- File exist
    -- Storing not existing directory in not existing directory
        UPDATE maja8801.VirtualDirectoryStored
            SET VirtualDirectoryID = 999997, TargetVirtualDirectoryID = 999998
        WHERE VirtualDirectoryID = 1; -- File exist
-- Checking triggers
        UPDATE maja8801.VirtualDirectoryStored
            SET TargetVirtualDirectoryID = 1
        WHERE VirtualDirectoryID = 1;
    -- Trying to store Documents inside Office directory, meanwhile Office is already stored in Documents 
        UPDATE maja8801.VirtualDirectoryStored
            SET TargetVirtualDirectoryID = 2
        WHERE VirtualDirectoryID = 1;
    -- Same to first test, but depth is 2 directories
        UPDATE maja8801.VirtualDirectoryStored
            SET VirtualDirectoryID = 6, TargetVirtualDirectoryID = 2
        WHERE VirtualDirectoryID = 1;