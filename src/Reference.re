type t('a) =
  | Mutable(ref('a))
  | Callable('a => unit)
;