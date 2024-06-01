CREATE FUNCTION UpdateDateModifiedFunction() 
   RETURNS TRIGGER 
   LANGUAGE PLPGSQL
AS $$
BEGIN
    NEW.DateModified := CURRENT_TIMESTAMP;
	RETURN NEW;
END;
$$;

-- Ensure that loop is not possible
CREATE FUNCTION CheckVirtualFileStoredFunction()
    RETURNS TRIGGER 
    LANGUAGE PLPGSQL
AS $$
DECLARE
    ParentDirID INTEGER;
BEGIN
    -- Trying store directory in it self
    IF (NEW.VirtualDirectoryID = NEW.TargetVirtualDirectoryID)  THEN
        RAISE EXCEPTION 'Cannot store virtual directory % in itself', NEW.VirtualDirectoryID;
    END IF;
    
    ParentDirID := NEW.TargetVirtualDirectoryID;
    
    WHILE ParentDirID IS NOT NULL LOOP
        IF ParentDirID = NEW.VirtualDirectoryID THEN
            RAISE EXCEPTION 'Cannot store virtual directory %, target directory is already stored in this directory', NEW.VirtualDirectoryID;
        END IF;
        
        -- Find the next parent in the hierarchy
        SELECT TargetVirtualDirectoryID INTO ParentDirID
            FROM VirtualDirectoryStored
            WHERE VirtualDirectoryID = ParentDirID;
    END LOOP;

    RETURN NEW;
END;
$$;

CREATE TRIGGER VirtualFileUpdateTrigger 
    BEFORE UPDATE
    ON VirtualFile
    FOR EACH ROW
        EXECUTE PROCEDURE UpdateDateModifiedFunction();

CREATE TRIGGER CheckVirtualFileStoredTrigger
    BEFORE UPDATE OR INSERT 
    ON VirtualDirectoryStored
    FOR EACH ROW
        EXECUTE PROCEDURE CheckVirtualFileStoredFunction();