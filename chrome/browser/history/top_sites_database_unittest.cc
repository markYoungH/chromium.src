// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <map>

#include "base/files/file_path.h"
#include "base/files/scoped_temp_dir.h"
#include "base/path_service.h"
#include "base/strings/utf_string_conversions.h"
#include "chrome/browser/history/top_sites_database.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/tools/profiles/thumbnail-inl.h"
#include "components/history/core/browser/history_types.h"
#include "sql/connection.h"
#include "sql/recovery.h"
#include "sql/test/scoped_error_ignorer.h"
#include "sql/test/test_helpers.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/sqlite/sqlite3.h"
#include "url/gurl.h"

namespace {

// URL with url_rank 0 in golden files.
const GURL kUrl0 = GURL("http://www.google.com/");

// URL with url_rank 1 in golden files.
const GURL kUrl1 = GURL("http://www.google.com/chrome/intl/en/welcome.html");

// URL with url_rank 2 in golden files.
const GURL kUrl2 = GURL("https://chrome.google.com/webstore?hl=en");

// Create the test database at |db_path| from the golden file at
// |ascii_path| in the "History/" subdir of the test data dir.
WARN_UNUSED_RESULT bool CreateDatabaseFromSQL(const base::FilePath &db_path,
                                                const char* ascii_path) {
  base::FilePath sql_path;
  if (!PathService::Get(chrome::DIR_TEST_DATA, &sql_path))
    return false;
  sql_path = sql_path.AppendASCII("History").AppendASCII(ascii_path);
  return sql::test::CreateDatabaseFromSQL(db_path, sql_path);
}

// Verify that the up-to-date database has the expected tables and
// columns.  Functional tests only check whether the things which
// should be there are, but do not check if extraneous items are
// present.  Any extraneous items have the potential to interact
// negatively with future schema changes.
void VerifyTablesAndColumns(sql::Connection* db) {
  // [meta] and [thumbnails].
  EXPECT_EQ(2u, sql::test::CountSQLTables(db));

  // Implicit index on [meta], index on [thumbnails].
  EXPECT_EQ(2u, sql::test::CountSQLIndices(db));

  // [key] and [value].
  EXPECT_EQ(2u, sql::test::CountTableColumns(db, "meta"));

  // [url], [url_rank], [title], [thumbnail], [redirects],
  // [boring_score], [good_clipping], [at_top], [last_updated], and
  // [load_completed], [last_forced]
  EXPECT_EQ(11u, sql::test::CountTableColumns(db, "thumbnails"));
}

void VerifyDatabaseEmpty(sql::Connection* db) {
  size_t rows = 0;
  EXPECT_TRUE(sql::test::CountTableRows(db, "thumbnails", &rows));
  EXPECT_EQ(0u, rows);
}

}  // namespace

namespace history {

class TopSitesDatabaseTest : public testing::Test {
 protected:
  void SetUp() override {
    // Get a temporary directory for the test DB files.
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    file_name_ = temp_dir_.path().AppendASCII("TestTopSites.db");
  }

  base::ScopedTempDir temp_dir_;
  base::FilePath file_name_;
};

// Version 1 is deprecated, the resulting schema should be current,
// with no data.
TEST_F(TopSitesDatabaseTest, Version1) {
  ASSERT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v1.sql"));

  TopSitesDatabase db;
  ASSERT_TRUE(db.Init(file_name_));
  VerifyTablesAndColumns(db.db_.get());
  VerifyDatabaseEmpty(db.db_.get());
}

TEST_F(TopSitesDatabaseTest, Version2) {
  ASSERT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v2.sql"));

  TopSitesDatabase db;
  ASSERT_TRUE(db.Init(file_name_));

  VerifyTablesAndColumns(db.db_.get());

  // Basic operational check.
  MostVisitedURLList urls;
  std::map<GURL, Images> thumbnails;
  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(3u, urls.size());
  ASSERT_EQ(3u, thumbnails.size());
  EXPECT_EQ(kUrl0, urls[0].url);  // [0] because of url_rank.
  // kGoogleThumbnail includes nul terminator.
  ASSERT_EQ(sizeof(kGoogleThumbnail) - 1,
            thumbnails[urls[0].url].thumbnail->size());
  EXPECT_TRUE(!memcmp(thumbnails[urls[0].url].thumbnail->front(),
                      kGoogleThumbnail, sizeof(kGoogleThumbnail) - 1));

  ASSERT_TRUE(db.RemoveURL(urls[1]));
  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(2u, urls.size());
  ASSERT_EQ(2u, thumbnails.size());
}

TEST_F(TopSitesDatabaseTest, Version3) {
  ASSERT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v3.sql"));

  TopSitesDatabase db;
  ASSERT_TRUE(db.Init(file_name_));

  VerifyTablesAndColumns(db.db_.get());

  // Basic operational check.
  MostVisitedURLList urls;
  std::map<GURL, Images> thumbnails;
  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(3u, urls.size());
  ASSERT_EQ(3u, thumbnails.size());
  EXPECT_EQ(kUrl0, urls[0].url);  // [0] because of url_rank.
  // kGoogleThumbnail includes nul terminator.
  ASSERT_EQ(sizeof(kGoogleThumbnail) - 1,
            thumbnails[urls[0].url].thumbnail->size());
  EXPECT_TRUE(!memcmp(thumbnails[urls[0].url].thumbnail->front(),
                      kGoogleThumbnail, sizeof(kGoogleThumbnail) - 1));

  ASSERT_TRUE(db.RemoveURL(urls[1]));
  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(2u, urls.size());
  ASSERT_EQ(2u, thumbnails.size());
}

// Version 1 is deprecated, the resulting schema should be current,
// with no data.
TEST_F(TopSitesDatabaseTest, Recovery1) {
  // Recovery module only supports some platforms at this time.
  if (!sql::Recovery::FullRecoverySupported())
    return;

  // Create an example database.
  EXPECT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v1.sql"));

  // Corrupt the database by adjusting the header size.
  EXPECT_TRUE(sql::test::CorruptSizeInHeader(file_name_));

  // Database is unusable at the SQLite level.
  {
    sql::ScopedErrorIgnorer ignore_errors;
    ignore_errors.IgnoreError(SQLITE_CORRUPT);
    sql::Connection raw_db;
    EXPECT_TRUE(raw_db.Open(file_name_));
    EXPECT_FALSE(raw_db.IsSQLValid("PRAGMA integrity_check"));
    ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
  }

  // Corruption should be detected and recovered during Init().
  {
    sql::ScopedErrorIgnorer ignore_errors;
    ignore_errors.IgnoreError(SQLITE_CORRUPT);

    TopSitesDatabase db;
    ASSERT_TRUE(db.Init(file_name_));
    VerifyTablesAndColumns(db.db_.get());
    VerifyDatabaseEmpty(db.db_.get());

    ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
  }
}

TEST_F(TopSitesDatabaseTest, Recovery2) {
  // Recovery module only supports some platforms at this time.
  if (!sql::Recovery::FullRecoverySupported())
    return;

  // Create an example database.
  EXPECT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v2.sql"));

  // Corrupt the database by adjusting the header.
  EXPECT_TRUE(sql::test::CorruptSizeInHeader(file_name_));

  // Database is unusable at the SQLite level.
  {
    sql::ScopedErrorIgnorer ignore_errors;
    ignore_errors.IgnoreError(SQLITE_CORRUPT);
    sql::Connection raw_db;
    EXPECT_TRUE(raw_db.Open(file_name_));
    EXPECT_FALSE(raw_db.IsSQLValid("PRAGMA integrity_check"));
    ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
  }

  // Corruption should be detected and recovered during Init().  After recovery,
  // the Version2 checks should work.
  {
    sql::ScopedErrorIgnorer ignore_errors;
    ignore_errors.IgnoreError(SQLITE_CORRUPT);

    TopSitesDatabase db;
    ASSERT_TRUE(db.Init(file_name_));

    VerifyTablesAndColumns(db.db_.get());

    // Basic operational check.
    MostVisitedURLList urls;
    std::map<GURL, Images> thumbnails;
    db.GetPageThumbnails(&urls, &thumbnails);
    ASSERT_EQ(3u, urls.size());
    ASSERT_EQ(3u, thumbnails.size());
    EXPECT_EQ(kUrl0, urls[0].url);  // [0] because of url_rank.
    // kGoogleThumbnail includes nul terminator.
    ASSERT_EQ(sizeof(kGoogleThumbnail) - 1,
              thumbnails[urls[0].url].thumbnail->size());
    EXPECT_TRUE(!memcmp(thumbnails[urls[0].url].thumbnail->front(),
                        kGoogleThumbnail, sizeof(kGoogleThumbnail) - 1));

    ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
  }
}

TEST_F(TopSitesDatabaseTest, Recovery3) {
  // Recovery module only supports some platforms at this time.
  if (!sql::Recovery::FullRecoverySupported())
    return;

  // Create an example database.
  EXPECT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v3.sql"));

  // Corrupt the database by adjusting the header.
  EXPECT_TRUE(sql::test::CorruptSizeInHeader(file_name_));

  // Database is unusable at the SQLite level.
  {
    sql::ScopedErrorIgnorer ignore_errors;
    ignore_errors.IgnoreError(SQLITE_CORRUPT);
    sql::Connection raw_db;
    EXPECT_TRUE(raw_db.Open(file_name_));
    EXPECT_FALSE(raw_db.IsSQLValid("PRAGMA integrity_check"));
    ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
  }

  // Corruption should be detected and recovered during Init().
  {
    sql::ScopedErrorIgnorer ignore_errors;
    ignore_errors.IgnoreError(SQLITE_CORRUPT);

    TopSitesDatabase db;
    ASSERT_TRUE(db.Init(file_name_));

    MostVisitedURLList urls;
    std::map<GURL, Images> thumbnails;
    db.GetPageThumbnails(&urls, &thumbnails);
    ASSERT_EQ(3u, urls.size());
    ASSERT_EQ(3u, thumbnails.size());
    EXPECT_EQ(kUrl0, urls[0].url);  // [0] because of url_rank.
    // kGoogleThumbnail includes nul terminator.
    ASSERT_EQ(sizeof(kGoogleThumbnail) - 1,
              thumbnails[urls[0].url].thumbnail->size());
    EXPECT_TRUE(!memcmp(thumbnails[urls[0].url].thumbnail->front(),
                        kGoogleThumbnail, sizeof(kGoogleThumbnail) - 1));

    ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
  }

  // Double-check database integrity.
  {
    sql::Connection raw_db;
    EXPECT_TRUE(raw_db.Open(file_name_));
    ASSERT_EQ("ok", sql::test::IntegrityCheck(&raw_db));
  }

  // Corrupt the thumnails.url auto-index by deleting an element from the table
  // but leaving it in the index.
  const char kIndexName[] = "sqlite_autoindex_thumbnails_1";
  // TODO(shess): Refactor CorruptTableOrIndex() to make parameterized
  // statements easy.
  const char kDeleteSql[] =
      "DELETE FROM thumbnails WHERE url = "
      "'http://www.google.com/chrome/intl/en/welcome.html'";
  EXPECT_TRUE(
      sql::test::CorruptTableOrIndex(file_name_, kIndexName, kDeleteSql));

  // SQLite can operate on the database, but notices the corruption in integrity
  // check.
  {
    sql::Connection raw_db;
    EXPECT_TRUE(raw_db.Open(file_name_));
    ASSERT_NE("ok", sql::test::IntegrityCheck(&raw_db));
  }

  // Open the database and access the corrupt index.
  {
    TopSitesDatabase db;
    ASSERT_TRUE(db.Init(file_name_));

    {
      sql::ScopedErrorIgnorer ignore_errors;
      ignore_errors.IgnoreError(SQLITE_CORRUPT);

      // Data for kUrl1 was deleted, but the index entry remains, this will
      // throw SQLITE_CORRUPT.  The corruption handler will recover the database
      // and poison the handle, so the outer call fails.
      EXPECT_EQ(TopSitesDatabase::kRankOfNonExistingURL,
                db.GetURLRank(MostVisitedURL(kUrl1, base::string16())));

      ASSERT_TRUE(ignore_errors.CheckIgnoredErrors());
    }
  }

  // Check that the database is recovered at the SQLite level.
  {
    sql::Connection raw_db;
    EXPECT_TRUE(raw_db.Open(file_name_));
    ASSERT_EQ("ok", sql::test::IntegrityCheck(&raw_db));
  }

  // After recovery, the database accesses won't throw errors.  The top-ranked
  // item is removed, but the ranking was revised in post-processing.
  {
    TopSitesDatabase db;
    ASSERT_TRUE(db.Init(file_name_));
    VerifyTablesAndColumns(db.db_.get());

    EXPECT_EQ(TopSitesDatabase::kRankOfNonExistingURL,
              db.GetURLRank(MostVisitedURL(kUrl1, base::string16())));

    MostVisitedURLList urls;
    std::map<GURL, Images> thumbnails;
    db.GetPageThumbnails(&urls, &thumbnails);
    ASSERT_EQ(2u, urls.size());
    ASSERT_EQ(2u, thumbnails.size());
    EXPECT_EQ(kUrl0, urls[0].url);  // [0] because of url_rank.
    EXPECT_EQ(kUrl2, urls[1].url);  // [1] because of url_rank.
  }
}

TEST_F(TopSitesDatabaseTest, AddRemoveEditThumbnails) {
  ASSERT_TRUE(CreateDatabaseFromSQL(file_name_, "TopSites.v3.sql"));

  TopSitesDatabase db;
  ASSERT_TRUE(db.Init(file_name_));

  // Add a new URL, not forced, rank = 1.
  GURL mapsUrl = GURL("http://maps.google.com/");
  MostVisitedURL url1(mapsUrl, base::ASCIIToUTF16("Google Maps"));
  db.SetPageThumbnail(url1, 1, Images());

  MostVisitedURLList urls;
  std::map<GURL, Images> thumbnails;
  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(4u, urls.size());
  ASSERT_EQ(4u, thumbnails.size());
  EXPECT_EQ(kUrl0, urls[0].url);
  EXPECT_EQ(mapsUrl, urls[1].url);

  // Add a new URL, forced.
  GURL driveUrl = GURL("http://drive.google.com/");
  MostVisitedURL url2(driveUrl, base::ASCIIToUTF16("Google Drive"));
  url2.last_forced_time = base::Time::FromJsTime(789714000000);  // 10/1/1995
  db.SetPageThumbnail(url2, TopSitesDatabase::kRankOfForcedURL, Images());

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(5u, urls.size());
  ASSERT_EQ(5u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);  // Forced URLs always appear first.
  EXPECT_EQ(kUrl0, urls[1].url);
  EXPECT_EQ(mapsUrl, urls[2].url);

  // Add a new URL, forced (earlier).
  GURL plusUrl = GURL("http://plus.google.com/");
  MostVisitedURL url3(plusUrl, base::ASCIIToUTF16("Google Plus"));
  url3.last_forced_time = base::Time::FromJsTime(787035600000);  // 10/12/1994
  db.SetPageThumbnail(url3, TopSitesDatabase::kRankOfForcedURL, Images());

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(6u, urls.size());
  ASSERT_EQ(6u, thumbnails.size());
  EXPECT_EQ(plusUrl, urls[0].url);  // New forced URL should appear first.
  EXPECT_EQ(driveUrl, urls[1].url);
  EXPECT_EQ(kUrl0, urls[2].url);
  EXPECT_EQ(mapsUrl, urls[3].url);

  // Change the last_forced_time of a forced URL.
  url3.last_forced_time = base::Time::FromJsTime(792392400000);  // 10/2/1995
  db.SetPageThumbnail(url3, TopSitesDatabase::kRankOfForcedURL, Images());

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(6u, urls.size());
  ASSERT_EQ(6u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);
  EXPECT_EQ(plusUrl, urls[1].url);  // Forced URL should have moved second.
  EXPECT_EQ(kUrl0, urls[2].url);
  EXPECT_EQ(mapsUrl, urls[3].url);

  // Change a non-forced URL to forced using UpdatePageRank.
  url1.last_forced_time = base::Time::FromJsTime(792219600000);  // 8/2/1995
  db.UpdatePageRank(url1, TopSitesDatabase::kRankOfForcedURL);

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(6u, urls.size());
  ASSERT_EQ(6u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);
  EXPECT_EQ(mapsUrl, urls[1].url);  // Maps moves to second forced URL.
  EXPECT_EQ(plusUrl, urls[2].url);
  EXPECT_EQ(kUrl0, urls[3].url);

  // Change a forced URL to non-forced using SetPageThumbnail.
  url3.last_forced_time = base::Time();
  db.SetPageThumbnail(url3, 1, Images());

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(6u, urls.size());
  ASSERT_EQ(6u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);
  EXPECT_EQ(mapsUrl, urls[1].url);
  EXPECT_EQ(kUrl0, urls[2].url);
  EXPECT_EQ(plusUrl, urls[3].url);  // Plus moves to second non-forced URL.

  // Change a non-forced URL to earlier non-forced using UpdatePageRank.
  db.UpdatePageRank(url3, 0);

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(6u, urls.size());
  ASSERT_EQ(6u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);
  EXPECT_EQ(mapsUrl, urls[1].url);
  EXPECT_EQ(plusUrl, urls[2].url);  // Plus moves to first non-forced URL.
  EXPECT_EQ(kUrl0, urls[3].url);

  // Change a non-forced URL to later non-forced using SetPageThumbnail.
  db.SetPageThumbnail(url3, 2, Images());

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(6u, urls.size());
  ASSERT_EQ(6u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);
  EXPECT_EQ(mapsUrl, urls[1].url);
  EXPECT_EQ(kUrl0, urls[2].url);
  EXPECT_EQ(plusUrl, urls[4].url);  // Plus moves to third non-forced URL.

  // Remove a non-forced URL.
  db.RemoveURL(url3);

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(5u, urls.size());
  ASSERT_EQ(5u, thumbnails.size());
  EXPECT_EQ(driveUrl, urls[0].url);
  EXPECT_EQ(mapsUrl, urls[1].url);
  EXPECT_EQ(kUrl0, urls[2].url);

  // Remove a forced URL.
  db.RemoveURL(url2);

  db.GetPageThumbnails(&urls, &thumbnails);
  ASSERT_EQ(4u, urls.size());
  ASSERT_EQ(4u, thumbnails.size());
  EXPECT_EQ(mapsUrl, urls[0].url);
  EXPECT_EQ(kUrl0, urls[1].url);
}

}  // namespace history
