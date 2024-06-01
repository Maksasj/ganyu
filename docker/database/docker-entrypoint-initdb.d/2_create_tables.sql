CREATE TABLE ganyu.Source(
    ID                      INT PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    sourceName              TEXT DEFAULT 'New Source',   
    sourceDescription       TEXT,
    sourceType              CHARACTER (16) NOT NULL,
    sourceRootDestination   TEXT NOT NULL

    CONSTRAINT SourceTypeConstraint 
        CHECK (sourceType = 'WEB' OR sourceType = 'PHYSICAL' OR sourceType = 'REMOTE')
);

CREATE TABLE ganyu.VirtualFile(
    ID              INT PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    FileName        TEXT DEFAULT 'New Virtual File',   
    FileExtension   TEXT DEFAULT NULL,  
    FileSize        INT,
    ContentHash     CHARACTER (32),
    DateCreated     DATE NOT NULL DEFAULT CURRENT_DATE,
    DateModified    DATE NOT NULL DEFAULT CURRENT_DATE,
    SourceID        INT NOT NULL,
    SourceAccess    TEXT NOT NULL,

    CONSTRAINT SourceIDConstraint
    FOREIGN KEY(SourceID)
        REFERENCES Source(ID)
        ON DELETE CASCADE,

    -- File name and extension can be null, but not at the same time
    CONSTRAINT VirtualFileNameConstraint 
    CHECK (
        ((FileName IS NOT NULL) AND (FileName != '')) OR 
        ((FileExtension IS NOT NULL) AND (FileExtension != ''))
    )
);

CREATE TABLE ganyu.VirtualDirectory(
    ID                      INT PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    directoryName           TEXT DEFAULT 'New Directory',   
    directoryDescription    TEXT
);

-- Create relation tables
CREATE TABLE ganyu.VirtualFileStored(
    VirtualFileID       INT NOT NULL,
    VirtualDirectoryID  INT NOT NULL,
    PRIMARY KEY (VirtualFileID, VirtualDirectoryID),

    CONSTRAINT VirtualFileIDConstraint
        FOREIGN KEY(VirtualFileID) 
        REFERENCES VirtualFile(ID)
        ON DELETE CASCADE,

    CONSTRAINT VirtualDirectoryIDConstraint
        FOREIGN KEY(VirtualDirectoryID) 
        REFERENCES VirtualDirectory(ID)
        ON DELETE CASCADE
);

CREATE TABLE ganyu.VirtualDirectoryStored(
    VirtualDirectoryID          INT NOT NULL,
    TargetVirtualDirectoryID    INT NOT NULL,
    PRIMARY KEY (VirtualDirectoryID, TargetVirtualDirectoryID),

    CONSTRAINT VirtualDirectoryIDConstraint
    FOREIGN KEY(VirtualDirectoryID) 
        REFERENCES VirtualDirectory(ID)
        ON DELETE CASCADE,

    CONSTRAINT TargetVirtualDirectoryIDConstraint
    FOREIGN KEY(TargetVirtualDirectoryID) 
        REFERENCES VirtualDirectory(ID)
        ON DELETE CASCADE
);