#!/usr/bin/env bash
set -u

DATA=20000records.tsv
LOG=valgrind_output.txt
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

# tiny inputs
printf "12.5683 55.6761\n139.6917 35.6895\n" > coords.txt
# read osm_id (tab-separated, column 4)
printf "214696\n162395\n145967\n" > ids.txt

bins=(
  id_query_naive
  id_query_indexed
  id_query_binsort_manual
  id_query_binsort_auto
  coord_query_naive_euclidean
  coord_query_naive_haversine
  coord_query_kdtree
)

# fresh log
{
  echo "== coords.txt =="; cat coords.txt; echo
  echo "== ids.txt ==";   cat ids.txt;   echo
} > "$LOG"

# run all, never abort early
for b in "${bins[@]}"; do
  if [[ -x "./$b" ]]; then
    {
      echo "------------------------------------------------"
      echo "Valgrind: $b"
      echo "------------------------------------------------"
      # coord bins use coords.txt; id bins use ids.txt
      in=coords.txt; [[ "$b" == id_* ]] && in=ids.txt
      $VG "./$b" "$DATA" < "$in"
      echo
    } >> "$LOG" 2>&1
  else
    echo "[SKIP] $b not built" >> "$LOG"
  fi
done

echo "Done. See $LOG"