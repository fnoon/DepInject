// depinject/di_test_impl.h

#ifndef NOON_DI_TEST_IMPL_H
#define NOON_DI_TEST_IMPL_H


class IBulb {
public:

  // Allow deletion through an interface pointer.
  virtual ~IBulb();

  // Forbid copying (slicing) via an interface pointer.
  IBulb(IBulb const&) = delete;
  IBulb& operator=(IBulb const&) = delete;

  // Non-virtual public interface.
  void electrified(bool receiving_current);

  bool is_lit() const;

protected:
  // Forbid instantiation of a bare interface class object.
  IBulb();

private:
  // Virtual hooks for derived class implementations.
  virtual void do_electrified(bool receiving_current) = 0;
  virtual bool do_is_lit() const = 0;
};


class Bulb : public IBulb {
public:
  Bulb();

  // Restore copy operations, overriding base class deletions.
  Bulb(Bulb const& b);
  Bulb& operator=(Bulb const& b);

private:
  virtual void do_electrified(bool receiving_current) override;
  virtual bool do_is_lit() const override;

  bool m_is_lit {false};
};


class Lamp {
public:
  Lamp();

  void toggle_switch();

  bool is_lit() const;

private:
  IBulb& m_bulb;
  bool   m_current_flowing {false};
};


#endif // NOON_DI_TEST_IMPL_H