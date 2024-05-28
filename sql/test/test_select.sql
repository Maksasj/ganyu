UPDATE maja8801.VirtualFile
    SET FileName = 'Changed file name lol'
WHERE ID = 1; -- File exist

REFRESH MATERIALIZED VIEW maja8801.LatestAccessVirtualFiles;

SELECT * FROM maja8801.LatestAccessVirtualFiles;
SELECT * FROM maja8801.WebVirtualFile;
SELECT * FROM maja8801.PhysicalVirtualFile;
SELECT * FROM maja8801.RemoteVirtualFile;

SELECT * FROM maja8801.Source;

SELECT * FROM maja8801.VirtualDirectory;

SELECT * FROM maja8801.VirtualFileStored;
SELECT * FROM maja8801.VirtualDirectoryStored;

SELECT * FROM maja8801.VirtualFile;