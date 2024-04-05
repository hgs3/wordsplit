#!/usr/bin/env bash
# This script builds wordsplit with Clang sanitizers.

# Run from the root directory.
cd ..

# Destination folder.
OUTDIR=analyze

# Don't accidentally overwrite the contents of a users working directory.
# Give them a chance to backup their work.
if [ -d "${OUTDIR}" ]; then
    echo "The '${OUTDIR}' directory already exists. Please rename or remove it."
    echo "If you did not create this directory, then it was probably created"
    echo "by the last execution of this script and is safe to manually delete."
    exit 1
fi

# All subsequent commands should kill the script on error.
set -e

# Clang sanitizers can only be run with Clang (duh).
if $CC |& grep -q "clang:"; then
    # Run the Undefined Behavior Sanitizer.
    cmake -B ${OUTDIR} -DUNDEFINED_BEHAVIOR_SANITIZER=ON
    cmake --build ${OUTDIR}
    ctest --test-dir ${OUTDIR}
    cmake -E remove_directory ${OUTDIR}

    # Run the Address Sanitizer.
    cmake -B ${OUTDIR} -DADDRESS_SANITIZER=ON
    cmake --build ${OUTDIR}
    ctest --test-dir ${OUTDIR}
    cmake -E remove_directory ${OUTDIR}

    # Run the Memory Sanitizer.
    cmake -B ${OUTDIR} -DMEMORY_SANITIZER=ON
    cmake --build ${OUTDIR}
    ctest --test-dir ${OUTDIR}
    cmake -E remove_directory ${OUTDIR}
else
    echo "Re-run with CC=Clang for UBSAN, ASAN, MSAN testing."
fi
