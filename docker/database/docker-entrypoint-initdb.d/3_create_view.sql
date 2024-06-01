-- Materialized view
CREATE MATERIALIZED VIEW LatestAccessVirtualFiles AS
    WITH LastAccess AS (
        SELECT VF.ID, GREATEST(VF.DateCreated, VF.DateModified) AS DateAccessed 
        FROM VirtualFile AS VF
    )
    SELECT LA.ID, LA.DateAccessed     
    FROM LastAccess AS LA
    WHERE LA.DateAccessed >= CURRENT_TIMESTAMP - INTERVAL '24 hours';

-- Regular views
CREATE VIEW WebVirtualFile AS
    SELECT VF.ID, VF.SourceID
    FROM VirtualFile AS VF 
    JOIN Source AS S 
        ON VF.SourceID = S.ID AND 
        S.sourceType = 'WEB';

CREATE VIEW PhysicalVirtualFile AS
    SELECT VF.ID, VF.SourceID
    FROM VirtualFile AS VF 
    JOIN Source AS S 
        ON VF.SourceID = S.ID AND 
        S.sourceType = 'PHYSICAL';

CREATE VIEW RemoteVirtualFile AS
    SELECT VF.ID, VF.SourceID
    FROM VirtualFile AS VF 
    JOIN Source AS S 
        ON VF.SourceID = S.ID AND 
        S.sourceType = 'REMOTE'