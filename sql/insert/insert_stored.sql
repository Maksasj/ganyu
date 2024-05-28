-- C++ files
INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
SELECT ID, 4
    FROM maja8801.VirtualFile
    WHERE FileExtension = '.cpp';

-- C files
INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
SELECT ID, 5
    FROM maja8801.VirtualFile
    WHERE FileExtension = '.c';

INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
SELECT ID, 3
    FROM maja8801.VirtualFile
    WHERE FileExtension = '.c';

INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
SELECT ID, 3
    FROM maja8801.VirtualFile
    WHERE FileExtension = '.cpp';

-- Personal word documents
INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
SELECT ID, 6
    FROM maja8801.VirtualFile
    WHERE FileExtension = '.docx';

-- All my photoshop files
INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
SELECT ID, 1
    FROM maja8801.VirtualFile
    WHERE FileExtension = '.psd';

-- Danboru drawings into drawing references
INSERT INTO maja8801.VirtualFileStored(VirtualFileID, VirtualDirectoryID)
WITH sources AS 
    (SELECT ID FROM maja8801.Source WHERE sourceType = 'WEB')
SELECT VF.ID, 2
    FROM maja8801.VirtualFile as VF
    JOIN sources ON sources.ID = VF.SourceID
    WHERE FileExtension = '.jpg' OR FileExtension = '.png';

INSERT INTO maja8801.VirtualDirectoryStored(VirtualDirectoryID, TargetVirtualDirectoryID)
VALUES
    (2, 1),
    (4, 3),
    (5, 3),
    (1, 6),
    (7, 6),
    (8, 7);
