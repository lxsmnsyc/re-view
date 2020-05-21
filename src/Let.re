let someOrError = (base: option('a), error: exn): 'a => {
  switch (base) {
    | Some(value) => value;
    | None => raise(error);
  }
};

module Option = {
  let let_ = (value: option('a), mapper: 'a => option('b)): option('b) => {
    switch (value) {
      | Some(actual) => mapper(actual);
      | None => None;
    }
  };
};

module OptionUnit = {
  let let_ = (value: option('a), mapper: 'a => 'b): unit => {
    switch (value) {
      | Some(actual) => ignore(mapper(actual));
      | None => ();
    }
  };
};

module OptionOrError = {
  let let_ = ((value, error): (option('a), exn), mapper: 'a => 'b): 'b => {
    switch (value) {
      | Some(actual) => mapper(actual);
      | None => raise(error);
    }
  };
};