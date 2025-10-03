#!/usr/bin/env bash
set -u

DATA=20000records.tsv
LOG=valgrind_output.txt
VG="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes"

# tiny inputs
printf "1.8753098 46.7995347\n1.8753098 46.7995347\n1.8753098 46.7995347\n1.8753098 46.7995347\n1.8753098 46.7995347\n
2.7473660 49.4333187\n2.7473660 49.4333187\n2.7473660 49.4333187\n2.7473660 49.4333187\n2.7473660 49.4333187\n
7.3223127 44.8045984\n7.3223127 44.8045984\n7.3223127 44.8045984\n7.3223127 44.8045984\n7.3223127 44.8045984\n
-79.7121523 55.4323463\n-79.7121523 55.4323463\n-79.7121523 55.4323463\n-79.7121523 55.4323463\n-79.7121523 55.4323463\n
3.7892891 47.4323246\n3.7892891 47.4323246\n3.7892891 47.4323246\n3.7892891 47.4323246\n3.7892891 47.4323246\n" > coords.txt
# read osm_id (tab-separated, column 4)
printf "2202162\n2202162\n2202162\n2202162\n2202162\n
1361397\n1361397\n1361397\n1361397\n1361397\n
43652\n43652\n43652\n43652\n43652\n
30113662\n30113662\n30113662\n30113662\n30113662\n30113662\n
3219806\n3219806\n3219806\n3219806\n3219806\n" > ids.txt

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