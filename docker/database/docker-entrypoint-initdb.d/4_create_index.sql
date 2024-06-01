CREATE INDEX VirtualFileSizeIndex
ON VirtualFile(FileSize);

CREATE INDEX VirtualFileContentHashIndex
ON VirtualFile(ContentHash);

CREATE UNIQUE INDEX VirtualFileContentAcrossSourcesIndex
ON VirtualFile(ContentHash, FileName, FileExtension, SourceID, SourceAccess);