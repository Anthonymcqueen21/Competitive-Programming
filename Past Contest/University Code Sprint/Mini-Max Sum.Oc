let int_list regex =
  List.map
    int_of_string
    (Core.Std.String.split (read_line ()) ~on:' ')

let space_separated_int_list () =
  int_list " "

let min_max_sum = function
  | [] | _ :: [] -> (0, 0)
  | x :: y :: [] -> (min x y, max x y)
  | head :: tail ->
    let rec min_max_sum_loc min max sum_min sum_max l = match l with
    | [] -> (sum_min, sum_max)
    | head :: tail ->
      let (min, add_to_min) =
        if head < min
        then (head, min)
        else (min, head)
      in
      let (max, add_to_max) =
        if head > max
        then (head, max)
        else (max, head)
      in
      min_max_sum_loc min max (sum_min + add_to_min) (sum_max + add_to_max) tail
    in
    min_max_sum_loc head head 0 0 tail

let () =
  let l = space_separated_int_list () in
  let (sum_min, sum_max) = min_max_sum l in
  Printf.printf "%d %d\n" sum_max sum_min
