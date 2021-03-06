#ifndef CHOOSER_T_HPP
#define CHOOSER_T_HPP

class chooser_t {
  public:
    virtual void next(void) = 0;
    virtual void prev(void) = 0;
    virtual void show(void) = 0;
    virtual void hide(void) = 0;
    virtual void east(void) = 0;
    virtual void west(void) = 0;
    virtual void north(void) = 0;
    virtual void south(void) = 0;
    virtual void raise(const unsigned int & window = 0) = 0;
    virtual void select(const unsigned int & window = 0) = 0;
    virtual void highlight(const unsigned int & window = 0) = 0;
};

#endif
