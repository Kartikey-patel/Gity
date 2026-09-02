#!/bin/bash

GITY="gity"
PASS=0
FAIL=0

echo "================================"
echo "        GITY TEST SUITE"
echo "================================"

test_init() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > output.txt 2>&1

    if [ -d ".gity" ] &&
       [ -d ".gity/objects" ] &&
       [ -d ".gity/refs" ] &&
       [ -f ".gity/HEAD" ] &&
       [ -f ".gity/index" ]; then
        echo "[PASS] init"
        ((PASS++))
    else
        echo "[FAIL] init"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}


test_basic_workflow() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    # Initialize repository
    $GITY init > /dev/null 2>&1

    # Create a file
    echo "Hello Gity" > hello.txt

    # Add
    $GITY add hello.txt > /dev/null 2>&1

    # Check that add succeeded by inspecting status
    STATUS=$($GITY status 2>&1)

    if ! echo "$STATUS" | grep -q 'hello.txt'; then
        echo "[FAIL] add"
        ((FAIL++))
        cd - > /dev/null
        rm -rf "$TEST_DIR"
        return
    fi

    echo "[PASS] add"
    ((PASS++))

    # Commit
    $GITY commit "initial commit" > /dev/null 2>&1

    # Check log
    LOG=$($GITY log 2>&1)

    if echo "$LOG" | grep -q 'initial commit'; then
        echo "[PASS] commit + log"
        ((PASS++))
    else
        echo "[FAIL] commit + log"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_clean_status() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Hello Gity" > hello.txt
    $GITY add hello.txt > /dev/null 2>&1
    $GITY commit "initial commit" > /dev/null 2>&1

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q "Changes to be committed" ||
       echo "$STATUS" | grep -q "Changes not staged for commit" ||
       echo "$STATUS" | grep -q "Untracked"; then
        echo "[FAIL] clean status"
        ((FAIL++))
    else
        echo "[PASS] clean status"
        ((PASS++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_unstaged_modification() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Version 1" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "initial commit" > /dev/null 2>&1

    # Modify working tree WITHOUT staging
    echo "Version 2" > file.txt

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'modified: "file.txt"'; then
        echo "[PASS] unstaged modification"
        ((PASS++))
    else
        echo "[FAIL] unstaged modification"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_staged_modification() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Version 1" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "initial commit" > /dev/null 2>&1

    # Modify and stage the change
    echo "Version 2" > file.txt
    $GITY add file.txt > /dev/null 2>&1

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'modified: "file.txt"'; then
        echo "[PASS] staged modification"
        ((PASS++))
    else
        echo "[FAIL] staged modification"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_staged_and_unstaged_modification() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Version 1" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "initial commit" > /dev/null 2>&1

    # Stage Version 2
    echo "Version 2" > file.txt
    $GITY add file.txt > /dev/null 2>&1

    # Working tree now becomes Version 3
    echo "Version 3" > file.txt

    STATUS=$($GITY status 2>&1)

    STAGED=$(echo "$STATUS" | grep -c 'Changes to be committed')
    UNSTAGED=$(echo "$STATUS" | grep -c 'Changes not staged for commit')

    if [ "$STAGED" -gt 0 ] && [ "$UNSTAGED" -gt 0 ]; then
        echo "[PASS] staged + unstaged modification"
        ((PASS++))
    else
        echo "[FAIL] staged + unstaged modification"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_staged_new_file() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "New file" > new.txt
    $GITY add new.txt > /dev/null 2>&1

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'new file: "new.txt"'; then
        echo "[PASS] staged new file"
        ((PASS++))
    else
        echo "[FAIL] staged new file"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_untracked_file() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "I am untracked" > untracked.txt

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'untracked.txt'; then
        echo "[PASS] untracked file"
        ((PASS++))
    else
        echo "[FAIL] untracked file"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_unstaged_deletion() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Delete me" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "initial commit" > /dev/null 2>&1

    # Delete from working tree, but don't stage the deletion
    rm file.txt

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'deleted: "file.txt"'; then
        echo "[PASS] unstaged deletion"
        ((PASS++))
    else
        echo "[FAIL] unstaged deletion"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_staged_deletion() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Delete me" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "initial commit" > /dev/null 2>&1

    # Delete file from working tree
    rm file.txt

    # Stage deletion
    $GITY add . > /dev/null 2>&1

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'deleted: "file.txt"'; then
        echo "[PASS] staged deletion"
        ((PASS++))
    else
        echo "[FAIL] staged deletion"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_staged_new_then_deleted() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Temporary file" > new.txt
    $GITY add new.txt > /dev/null 2>&1

    rm new.txt

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'new file: "new.txt"' &&
       echo "$STATUS" | grep -q 'deleted: "new.txt"'; then
        echo "[PASS] staged new then deleted"
        ((PASS++))
    else
        echo "[FAIL] staged new then deleted"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_gityignore() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "ignored.txt" > .gityignore
    echo "Do not track me" > ignored.txt
    echo "Track me" > normal.txt

    $GITY add . > /dev/null 2>&1

    STATUS=$($GITY status 2>&1)

    if echo "$STATUS" | grep -q 'ignored.txt'; then
        echo "[FAIL] .gityignore"
        echo "$STATUS"
        ((FAIL++))
    elif echo "$STATUS" | grep -q 'normal.txt'; then
        echo "[PASS] .gityignore"
        ((PASS++))
    else
        echo "[FAIL] .gityignore"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_multiple_commits() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Version 1" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "first commit" > /dev/null 2>&1

    echo "Version 2" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "second commit" > /dev/null 2>&1

    echo "Version 3" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "third commit" > /dev/null 2>&1

    LOG=$($GITY log 2>&1)

    if echo "$LOG" | grep -q 'first commit' &&
       echo "$LOG" | grep -q 'second commit' &&
       echo "$LOG" | grep -q 'third commit'; then
        echo "[PASS] multiple commits"
        ((PASS++))
    else
        echo "[FAIL] multiple commits"
        echo "$LOG"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_persistence() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    echo "Version 1" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "first commit" > /dev/null 2>&1

    echo "Version 2" > file.txt
    $GITY add file.txt > /dev/null 2>&1
    $GITY commit "second commit" > /dev/null 2>&1

    # Fresh CLI invocation: read repository state again
    LOG=$($GITY log 2>&1)
    STATUS=$($GITY status 2>&1)

    if echo "$LOG" | grep -q 'first commit' &&
       echo "$LOG" | grep -q 'second commit' &&
       ! echo "$STATUS" | grep -q 'Changes to be committed' &&
       ! echo "$STATUS" | grep -q 'Changes not staged for commit'; then
        echo "[PASS] persistence"
        ((PASS++))
    else
        echo "[FAIL] persistence"
        echo "$LOG"
        echo "$STATUS"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_add_nonexistent_file() {
    TEST_DIR=$(mktemp -d)
    cd "$TEST_DIR" || exit 1

    $GITY init > /dev/null 2>&1

    OUTPUT=$($GITY add does-not-exist.txt 2>&1)
    EXIT_CODE=$?

    if [ "$EXIT_CODE" -ne 0 ]; then
        echo "[PASS] add nonexistent file"
        ((PASS++))
    else
        echo "[FAIL] add nonexistent file"
        echo "$OUTPUT"
        ((FAIL++))
    fi

    cd - > /dev/null || exit 1
    rm -rf "$TEST_DIR"
}

test_init
test_basic_workflow
test_clean_status
test_unstaged_modification
test_staged_modification
test_staged_and_unstaged_modification
test_staged_new_file
test_untracked_file
test_unstaged_deletion
test_staged_deletion
test_staged_new_then_deleted
test_gityignore
test_multiple_commits
test_persistence
test_add_nonexistent_file

echo
echo "================================"
echo "Passed: $PASS"
echo "Failed: $FAIL"
echo "================================"
